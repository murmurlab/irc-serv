#include "Evaluator.hpp"
#include "Channel.hpp"
#include "Instruction.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Lexer.hpp"
#include "irc.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstddef>
#include <sstream>
#include <string>
#include <unistd.h>

using std::find_first_of;

Evaluator::Evaluator(Lexer &lexer_, Client &me_) : _lexer(lexer_), _me(me_) {}

Instruction	&Evaluator::newInstruction() {
	promises.resize(promises.size() + 1);
	return promises.back();
}

void	Evaluator::_CMD_needarg(Message &msg) {
	Instruction	&res = newInstruction();
	
	res.msg.prefix.host = "source-of-server";
	res.msg.command = "461";
	res.msg.params.push_back(_me.nickname);
	res.msg.params.push_back(res.msg.command);
	res.msg.trailing = "Not enough parameters";
}

void	Evaluator::_CMD_unknown(Message &msg) {
	Instruction	&res = newInstruction();

	res.opr = SEND;
	res.msg.prefix.host = "source-of-server";
	res.msg.command = "421";
	res.msg.params.push_back(_me.nickname);
	res.msg.params.push_back(msg.command);
	res.msg.trailing = "Unknown command";	
}

void	Evaluator::_CAP_END(Message &msg) {
	Instruction	&res = newInstruction();

	res.msg.command = "";
}

void	Evaluator::_CAP_LS(Message &msg) {
	Instruction	&res = newInstruction();

	res.opr = SEND;
	res.msg.command = "CAP";
	res.msg.params.push_back("*");
	res.msg.params.push_back("LS");
	if (msg.params.size() < 3) {
		// no ver
	} else if (msg.params[2] == "302") {
		// ver 302
	} else {
		// unknown ver
		// ver default=302
	}
	res.msg.trailing = "sasl";
}

void	Evaluator::_CAP_REQ(Message &msg) {	
	Instruction	&res = newInstruction();

	if (msg.params.size() == 1) {
		stringstream	trailing;
		int				not_found = 1;

		trailing.str(msg.trailing);
		trailing.get();
		for (string param; !trailing.eof();trailing >> param) {
			trailing >> param;
			if (param == "sasl") {
				not_found = 0;
			}
		}
		res.msg.params.push_back("*");
		if (not_found) {
			res.msg.params.push_back("NAK");
			// merge all trail params
			res.msg.trailing = msg.trailing;
		} else {
			res.msg.params.push_back("ACK");
			res.msg.trailing = "sasl";
		}
	} else if (msg.params.size() == 2) {
		if (msg.params[1] == "sasl") {
			res.msg.params.push_back("ACK");
			res.msg.params.push_back("sasl");
		} else {
			res.msg.params.push_back("NAK");
			res.msg.trailing = msg.params[1];
			// non supported cap
		}
	} else if (msg.params.size() > 2) {
		// unknown syntax
	}
}

void	Evaluator::_CAP_unk(Message &msg) {
	Instruction	&res = newInstruction();

	res.msg.prefix.host = "example.org";
	res.msg.params.push_back("410");
	res.msg.params.push_back("*");
	res.msg.params.push_back(msg.params[0]);
	res.msg.trailing = "Invalid CAP command";
	// Server: :example.org 410 jw FOO :Invalid CAP command
}

void	Evaluator::_CAP(Message &msg)
{
	if (msg.params.size() == 0) {
		_CMD_needarg(msg);
		return ;
	}
	if (msg.params[0] == "END") {
		_CAP_END(msg);
		return ;
	}
	if (msg.params[0] == "LS"/*  || msg.params[0] == "LIST" */) {
		_CAP_LS(msg);
		return ;
	}
	if (msg.params[0] == "REQ") {
		_CAP_REQ(msg);
		return ;
	}
	_CAP_unk(msg);
	return ;
}

void Evaluator::_NICK(Message &msg) {
	// Instruction	&res = newInstruction();
	static string	nick_chars(NICK_CHARS);

	if (msg.params.size() == 0) {
		Instruction	&res = newInstruction();
		res.opr = SEND;
		res.msg.command = "431";
		res.msg.params.push_back(_me.nickname);
		res.msg.trailing = "No nickname given";
		return ;
	}
	// 432
	if (msg.params[0].find_first_not_of(nick_chars) != string::npos) {
		Instruction	&res = newInstruction();
		res.opr = SEND;
		res.msg.command = "432";
		res.msg.params.push_back(_me.nickname);
		res.msg.params.push_back(msg.params[0]);
		res.msg.trailing = "Erroneous nickname";
		return ;
	}
	// 433
	if (_me._server.getClientByNick(msg.params[0]) != NULL) {
		Instruction	&res = newInstruction();
		res.opr = SEND;
		res.msg.command = "433";
		res.msg.params.push_back(_me.nickname);
		res.msg.params.push_back(msg.params[0]);
		res.msg.trailing = "Nickname is already in use";
		return ;
	}
	if (_me.nickname.empty()) {
		_me.nickname = msg.params[0];
		return ;
	}
	{
		Instruction	&res = newInstruction();
		res.opr = SEND;
		res.msg.command = "NICK";
		res.msg.params.push_back(msg.params[0]);
		res.msg.prefix.nick = _me.nickname;
		res.msg.prefix.user = _me.username;
		res.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
	}
	_me.nickname = msg.params[0];
}

void Evaluator::_USER(Message &msg) {
	// Instruction	&res = newInstruction();

	if (msg.params.size() <= 3) {
		if (msg.params.size() == 3 && msg.trailing.empty()) {
			_CMD_needarg(msg);
			return ;
		}
	}
	// ERR_ALREADYREGISTRED
	if (!_me.username.empty()) {
		Instruction	&res = newInstruction();
		res.opr = SEND;
		res.msg.command = "462";
		res.msg.params.push_back(_me.nickname);
		res.msg.trailing = "Unauthorized command (already registered)";
		return ;
	}
	_me.username = msg.params[0];
	_me.realname = msg.trailing;
	{
		Instruction	&res = newInstruction();
		res.opr = SEND;
		// res.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
		// res.msg.prefix.nick = _me.nickname;
		// res.msg.prefix.user = _me.username;
		res.msg.command = "001";
		res.msg.params.push_back(_me.nickname);
		res.msg.trailing = "Welcome to the Internet Relay Network " + _me.nickname + "!" + _me.username + "@" + inet_ntoa(_me.addr.sin_addr);
	}

}

void Evaluator::_JOIN(Message &msg) {
	stringstream	keys;
	stringstream	channels;
	string			channel;
	string			key;
	int	has_key;

	has_key = 0;
	if (msg.params.size() == 0) {
		_CMD_needarg(msg);
		return ;
	}
	if (msg.params.size() > 1) {
		has_key = 1;
		keys.str(msg.params[1]);
	}
	channels.str(msg.params[0]);
	while (getline(channels, channel, ',')) {
		cout << "channel: " << channel << " key: " << key << endl;
		switch (has_key) {
			case 1: if (getline(keys, key, ','));else has_key = 0;
		}
		{
			Instruction	&res = newInstruction();
			res.opr = SEND;
			// res.msg.prefix.host = inet_ntoa(_me._server._listen_addr.sin_addr);
			res.msg.params.push_back(_me.nickname);
			res.msg.params.push_back(channel);
			switch (_me._server.join_ch(_me, channel, key)) {
				case SUCCESS: // emit or reply ???
				// cout << "channel: " << channel << " key: " << key << endl;
				res.msg.prefix.nick = _me.nickname;
				// cout << "nick and user: " << _me.nickname << " " << _me.username << endl;
				res.msg.prefix.user = _me.username;
				res.msg.prefix.host = "127.0.0.1";
				res.msg.params.erase(res.msg.params.begin());
				// res.msg.params.erase(res.msg.params.begin());
				// res.msg.trailing = channel;
				res.msg.command = "JOIN";
				// res.opr = VOID;
				{
					Instruction	&res2 = newInstruction();
					res2.opr = SEND;
					// res2.msg.prefix.nick = _me.nickname;
					// res2.msg.prefix.user = _me.username;
					// res2.msg.prefix.host = "127.0.0.1"; //inet_ntoa(_me.addr.sin_addr);
					res2.msg.params.push_back(_me.nickname);
					res2.msg.params.push_back(channel);
					if (_me._server.getChannelByName(channel)->topic.empty()) {
						res2.msg.command = ": 332";
						res2.msg.trailing = "No topic is set";
					} else {
						res2.msg.command = "332";
						res2.msg.trailing = _me._server.getChannelByName(channel)->topic;
					}
				}
				{
					Instruction	&res3 = newInstruction();
					res3.opr = SEND;
					res3.msg.prefix.nick = _me.nickname;
					res3.msg.prefix.user = _me.username;
					res3.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
					res3.msg.command = "353";
					res3.msg.params.push_back("=" + channel);
					for (std::vector<Client *>::size_type i = 0; i < _me._server.getChannelByName(channel)->clients.size(); i++) {
						res3.msg.trailing += _me._server.getChannelByName(channel)->clients[i]->nickname + " ";
					}
					// res3.msg.trailing = "testuser1 testuser2";
				}
				{
					Instruction	&res4 = newInstruction();
					res4.opr = SEND;
					res4.msg.prefix.nick = _me.nickname;
					res4.msg.prefix.user = _me.username;
					res4.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
					res4.msg.command = "366";
					res4.msg.params.push_back(channel);
					res4.msg.trailing = "End of NAMES list.";
				}
				// res.msg.command = "JOIN";
				// res.opr = VOID;
				break ;
				case ERR_NOSUCHCHANNEL:
				res.msg.command = "403";
				res.msg.trailing = "No such channel";
				break ;
				case ERR_BADCHANNELKEY:
				res.msg.command = "475";
				res.msg.trailing = "Cannot join channel (+k)";
				break ;
				case ERR_CHANNELISFULL:
				res.msg.command = "471";
				res.msg.trailing = "Cannot join channel (+l)";
				break ;
				case ERR_INVITEONLYCHAN:
				res.msg.command = "473";
				res.msg.trailing = "Cannot join channel (+i)";
				break ;
			}
		}
	}
}
	// res.opr = SEND;
	// res.msg.prefix.nick = _me.nickname;
	// res.msg.prefix.user = _me.username;
	// res.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
	// res.msg.command = "JOIN";
	// if (msg.params[0].empty())
	// {
	// 	res.msg.command = "462";
	// 	res.msg.params.push_back(_me.nickname);
	// 	res.msg.trailing = "Unauthorized command (already registered)";
	// 	return ;
	// }
	// else
	// 	res.msg.trailing = msg.params[0];

	// Instruction	&res2 = newInstruction();
	// res2.opr = VOID;
	// res2.msg.command = "332";
    // res2.msg.params.push_back(_me.nickname);
    // res2.msg.params.push_back(msg.params[0]);
    // res2.msg.trailing = "test channel";

	// Instruction	&res3 = newInstruction();
    // // Kanal üyeleri
	// res3.opr = VOID;
    // res3.msg.command = "353";
    // res3.msg.params.push_back(_me.nickname);
    // res3.msg.params.push_back("=");
    // res3.msg.trailing = "testuser1 testuser2";

    // // Kanalın sonu
	// Instruction res4 = newInstruction();
	// res4.opr = VOID;
    // res4.msg.command = "366";
    // res4.msg.params.push_back(_me.nickname);
    // res4.msg.params.push_back(msg.params[0]);
    // res4.msg.trailing = "End of /NAMES list.";

	//EKLENCEKLER
	// PARAMS[1] kısmıyla şifre gelebilir kanalları ayarlıyınca ele alınmalı
	//KANALLAR AYARLANDIKTAN SONRA EKLENCEKLER
	//:ServerName 332 Nick #kanal :Kanalın mevcut konusu
	// :ServerName 353 Nick = #kanal :@User1 +User2 User3
	//:ServerName 366 Nick #kanal :End of /NAMES list.

	//ERROR
	//:ServerName 403 Nick #kanal :No such channel
	//:ServerName 475 Nick #kanal :Cannot join channel (+k)
	//:ServerName 471 Nick #kanal :Cannot join channel (+l)


void	Evaluator::_PASS(Message &msg) {
	if (msg.params.size() == 0 && msg.trailing.empty()) {
		_CMD_needarg(msg);
		return ;
	}
	if (_me.authorized) {
		Instruction	&res = newInstruction();

		res.opr = SEND;
		res.msg.command = "462";
		res.msg.params.push_back(_me.nickname);
		res.msg.trailing = "Unauthorized command (already registered)";
		return ;
	}
	_me.authorized = _me._server.authorize(msg.trailing);
	if (_me.authorized)
		return ;
	Instruction	&res = newInstruction();

	res.opr = SEND;
	res.msg.command = "464";
	res.msg.params.push_back(_me.nickname);
	res.msg.trailing = "Password incorrect";
	// Instruction	&res = newInstruction();

	// res.opr = SEND;
	// res.msg.command = "001";
	// res.msg.params.push_back(_me.nickname);
	// res.msg.trailing = res.msg.trailing +
	// 	"Welcome to the Internet Relay Network " +
	// 	_me.nickname + "!" + _me.username + "@" +
	// 	inet_ntoa(_me.addr.sin_addr);
	// res.opr = VOID;
}

void Evaluator::_NOTICE(Message &msg)
{
	Instruction	&res = newInstruction();

	res.opr = VOID;
	res.msg.command = "NOTICE";
	res.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
	res.msg.params = msg.params;
	res.msg.trailing = msg.trailing;
}

void Evaluator::_PING(Message &msg)
{
	Instruction	&res = newInstruction();

	res.opr = VOID;
	res.msg.command = "PONG";
	res.msg.trailing = msg.trailing;
	res.msg.prefix = msg.prefix;
	res.msg.params = msg.params;

}

void Evaluator::_LIST(Message &msg)
{
	Instruction	&res = newInstruction();

	res.opr = VOID;
	res.msg.command = "LIST";
	res.msg.params.push_back("#test");
}

void Evaluator::_QUIT(Message &msg)
{
	Instruction	&res = newInstruction();

	res.opr = EMIT;
	
	res.msg.prefix.host = inet_ntoa(_me.addr.sin_addr);
	res.msg.prefix.user = _me.username;
	res.msg.prefix.nick = _me.nickname;
	
	res.msg.command = "QUIT";
	res.msg.trailing = msg.trailing;
}

int Evaluator::_evalOne(std::list<Message> &msgs) 
{
	Message	*msg;

	if (msgs.empty())
		return 0;
	msg = &(msgs.front());
	
	void (Evaluator::*funcs[])(Message &msg) = {&Evaluator::_CAP, &Evaluator::_PASS,\
	&Evaluator::_QUIT, &Evaluator::_PING, &Evaluator::_JOIN, &Evaluator::_LIST, &Evaluator::_NOTICE, &Evaluator::_NICK, &Evaluator::_USER};
	std::string cmds[] = {"CAP", "PASS", "QUIT", "PING", "JOIN", "LIST", "NOTICE", "NICK", "USER"};

	switch (_me.authorized) {
		case 0:
		if (msg->command != "PASS") {
			if (msg->command != "CAP" && msg->command != "PING") {
				// not authorized
				cout << "not authorized: " << msg->command << endl;
				msgs.pop_front();
				return 1;
			}
		}
		break;
	}
	for (int x = 0; x < sizeof(cmds) / sizeof(cmds[0]); x++)
	{
		// cout << "x: " << x << " cmd: " << cmds[x] << " msg: " << msg->command << endl;
		if (x == sizeof(cmds) / sizeof(cmds[0])){
        	_CMD_unknown(*msg);
        	break;
    	}
    	if (msg->command == cmds[x]) {
        	(this->*funcs[x])(*msg);
        	break;
    	}
	}
	msgs.pop_front();
	return 1;
}

void	Evaluator::eval() {
	
	// try {
	// 	_lexer.lex();
	// } catch (IRC_MsgIncomplate& e) {
	// 	cout << e.what() << ": Evaluator::eval(): IRC_MsgIncomplate" << endl<< endl;
	// }
	// _matchAll(_parser.msgs);
	while (_evalOne(_lexer.msgs));
}
