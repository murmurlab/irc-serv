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
		if (req.params[x] == "ZmZmAGZmZgAxMjM0")
		{
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

	if (msg.command != "CAP")
		return res;
	else
		res.command = "CAP";
	res.params.push_back("*");
	for (int x = 0; x < msg.params.size(); x++)
	{
		if (msg.params[x] == "LS")
				res.params.push_back("LS"), \
					res.trailing = new string("sasl");
		if (msg.params[x++] == "REQ")
		{
			if (msg.params[x] != "sasl")
			{
				res.params.clear();
				res.params.push_back("NAK");
				if (res.trailing)
					delete res.trailing , \
						res.trailing = new string(msg.params[x]);
				else
					res.trailing = new string (msg.params[x]);
			}
			if (msg.params[x] == "sasl")
			{
				res.params.push_back("ACK");
				if (res.trailing)
					delete res.trailing , \
						res.trailing = new string(msg.params[x]);
				else
					res.trailing = new string (msg.params[x]);
			}
		}
	}
	return (res);
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
			cout << "register user" << endl;
		} else if (msg->command == "AUTHENTICATE")
			_AUTH(*msg, *res);		
		else {
			responses.resize(responses.size() - 1);
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
	ret += "\r\n";
	std::cout << "------------ " << ret << " -------" << endl;
	return ret;
	// write(_desc, ret.c_str(), strlen(ret.c_str()));

	return "";
}

void	Executer::execute() {
	try {
		_parser.parse();
	} catch (IRC_MsgIncomplate& e) {
		cout << e.what() << ": Executer::execute(): IRC_MsgIncomplate" << endl;
	}
	// _matchAll(_parser.msgs);
	while (_matchOne(_parser.msgs));
}
