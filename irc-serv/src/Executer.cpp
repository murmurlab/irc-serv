#include "Executer.hpp"
#include "Message.hpp"
#include "Parser.hpp"

Executer::Executer(Parser &parser_, Client &me_): _parser(parser_), _me(me_) {

}

// void testResponse(Message &req, int _desc)
// {
// 	string res;
// 	goto test;
// 	if (!req.prefix.user->empty())
// 	{
// 		res = ":";
// 		if (!req.prefix.nick->empty())
// 			res += *req.prefix.nick;
// 		else
// 			res += *req.prefix.nick + "!" \
// 				+ *req.prefix.user + " ";
// 	}
// 	test:
// 	if (!req.command.empty())
// 		res += req.command + " ";
// 	for (int x = 0; x < req.params.size(); x++)
// 		res += req.params[x] + " ";
// 	if (req.trailing)
// 		res += ":" + *req.trailing;
// 	res += "\r\n";
// 	std::cout << "------------ " << res << " -------" << endl;
// 	write(_desc, res.c_str(), strlen(res.c_str()));
// }

Message &_AUTH(Message &req,Message &retRequest)
{
	if (req.command != "AUTHENTICATE")
		return (retRequest);
	else
		retRequest.command = "AUTHENTICATE";
	for (int x = 0; x < req.params.size(); x++)
	{
		if (req.params[x] == "PLAIN")
			retRequest.params.push_back("+");
		if (req.params[x] == "ZmZmAGZmZgAxMjM0" ||
			req.params[x] == "c2FzbG5pY2sAc2FzbG5pY2sAMTIzNA==")
		{
			// send these for CAP END
			// S: :jaguar.test 900 jilles jilles!jilles@localhost.stack.nl jilles :You are now logged in as jilles
			// S: :jaguar.test 903 jilles :SASL authentication successful
			// then 001
			retRequest.command = "001";
			retRequest.params.clear();
			if (!retRequest.trailing)
				retRequest.trailing = new string("Welcome FT_IRC");
			break ;
		}
	}
	return (retRequest);
}

Message &_CAP(Message &msg, Message &res)
{
	res.command = "CAP";
	if (msg.params.size() == 0) {
		// no subcommand
		res.prefix.host = new string("example.org");
		res.params.push_back("410");
		res.params.push_back("*");
		res.params.push_back("");
		res.trailing = new string("Invalid CAP command");

		// res.prefix.host = new string("example.org");
		// res.params.push_back("410");
		// res.params.push_back("*");
		// res.params.push_back(msg.params[0]);
		// res.trailing = new string(":Invalid CAP command");
		
		// Server: :example.org 410 jw FOO :Invalid CAP command
	} else if (msg.params[0] == "LS"/*  || msg.params[0] == "LIST" */) {
		res.params.push_back("*");
		res.params.push_back("LS");
		if (msg.params.size() < 3) {
			// no ver
		} else if (msg.params[2] == "302") {
			// ver 302
		} else {
			// unknown ver
			// ver default=302
		}
		res.trailing = new std::string("sasl");
	} else if (msg.params[0] == "REQ") {
		// res.params.push_back("*");
		// res.params.push_back("LS");
		
		if (msg.params.size() == 1) {
			stringstream	trailing;
			int				not_found = 1;
			
			trailing.str(*msg.trailing);
			trailing.get();
			for (string param; !trailing.eof();trailing >> param) {
				trailing >> param;
				if (param == "sasl") {
					not_found = 0;
				}
			}
			res.params.push_back("*");
			if (not_found) {
				res.params.push_back("NAK");
				// merge all trail params
				res.trailing = new string("-all caps-");
			} else {
				res.params.push_back("ACK");
				res.trailing = new string("sasl");
			}
		} else if (msg.params.size() == 2) {
			if (msg.params[1] == "sasl") {
				res.params.push_back("ACK");
				res.params.push_back("sasl");
			} else {
				res.params.push_back("NAK");
				res.trailing = new string(msg.params[1]);
				// non supported cap
			}
		} else if (msg.params.size() > 2) {
			// unknown syntax
		}
	} else {
		res.prefix.host = new string("example.org");
		res.params.push_back("410");
		res.params.push_back("*");
		res.params.push_back(msg.params[0]);
		res.trailing = new string("Invalid CAP command");
		// Server: :example.org 410 jw FOO :Invalid CAP command
	}
	return res;
}

void Executer::_matchAll(std::list<Message> &msgs) {
	Message	*res;
	
	res = NULL;
	for (Message *msg = NULL; !msgs.empty(); ) {
		msg = &(msgs.back());
		responses.resize(responses.size() + 1);
		res = &responses.back();

		if (msg->command == "CAP") {
			// testResponse(_CAP(*msg, *res), _gls._desc);
			_CAP(*msg, *res);
		} else if (msg->command == "USER") {
			_me.username = *(msg->params.end() - 1);
			cout << "register user" << endl;
		} else if (msg->command == "AUTHENTICATE")
			_AUTH(*msg, *res);
		else {
			res->command = "421";
		}
		msgs.pop_back();
	}
}

int Executer::_matchOne(std::list<Message> &msgs) {
	Message	*res, *msg;
	
	if (msgs.empty())
		return 0;
	msg = &(msgs.back());
	responses.resize(responses.size() + 1);
	res = &responses.back();

	if (msg->command == "CAP") {
		_CAP(*msg, *res);
	} else if (msg->command == "USER") {
		cout << "register user" << endl;
	} else if (msg->command == "AUTHENTICATE")
		_AUTH(*msg, *res);
	else {
		responses.resize(responses.size() - 1);
	}
	msgs.pop_back();
	return 1;
}

string Executer::_serialize(Message const &res) {
	string ret;
	goto test;
	// if (!req.prefix.user->empty())
	// {
	// 	res = ":";
	// 	if (!req.prefix.nick->empty())
	// 		res += *req.prefix.nick;
	// 	else
	// 		res += *req.prefix.nick + "!" \
	// 			+ *req.prefix.user + " ";
	// }

	test:
	if (!res.command.empty())
		ret += res.command + " ";
	for (int x = 0; x < res.params.size(); x++)
		ret += res.params[x] + " ";
	if (res.trailing)
		ret += ":" + *res.trailing;
	std::cout << "<" << "\"" << ret << "\"" << endl<< endl;
	ret += "\r\n";
	return ret;
	// write(_desc, ret.c_str(), strlen(ret.c_str()));

	return "";
	operator new[](3, NULL);
}

void	Executer::execute() {
	try {
		_parser.parse();
	} catch (IRC_MsgIncomplate& e) {
		cout << e.what() << ": Executer::execute(): IRC_MsgIncomplate" << endl<< endl;
	}
	// _matchAll(_parser.msgs);
	while (_matchOne(_parser.msgs));
}
