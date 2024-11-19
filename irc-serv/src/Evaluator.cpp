#include "Evaluator.hpp"
#include "Instruction.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include "Lexer.hpp"
#include <arpa/inet.h>
#include <unistd.h>

Evaluator::Evaluator(Lexer &lexer_, Client &me_): _lexer(lexer_), _me(me_) {

}

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

void	Evaluator::_PASS(Message &msg) {
	Instruction	&res = newInstruction();

	res.opr = SEND;
	if (_me.authorized) {
		res.msg.command = "462";
		res.msg.params.push_back(_me.nickname);
		res.msg.trailing = "Unauthorized command (already registered)";
		return ;
	}
	_me.authorized = _me._server.authorize(msg.trailing);
	if (!_me.authorized) {
		res.msg.command = "464";
		res.msg.params.push_back(_me.nickname);
		res.msg.trailing = "Password incorrect";
		return ;
	}
	res.msg.command = "001";
	res.msg.params.push_back(_me.nickname);
	res.msg.trailing = res.msg.trailing +
		"Welcome to the Internet Relay Network " +
		_me.nickname + "!" + _me.username + "@" +
		inet_ntoa(_me.addr.sin_addr);
}

int Evaluator::_evalOne(std::list<Message> &msgs) {
	Message	*msg;

	if (msgs.empty())
		return 0;
	msg = &(msgs.front());
	
	if (msg->command == "CAP") {
		_CAP(*msg);
	} else if (msg->command == "PASS") {
		_PASS(*msg);
	} else if (msg->command == "NICK") {
		// NICK(*msg, *res);
	} else if (msg->command == "USER") {
		// _USER(*msg, *res);
	} else {
		_CMD_unknown(*msg);
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
