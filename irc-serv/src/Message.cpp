#include "Message.hpp"
#include <iostream>

using std::endl;

Message::Message() {
  // std::cout << "Message construct" << std::endl;
}

Message::~Message() {
}

string Message::_serialize(Message &res) {
	string ret;
	if (!res.prefix.nick.empty()) {
		ret += ":" + res.prefix.nick;
		if (!res.prefix.user.empty())
			ret += "!" + res.prefix.user;
		if (!res.prefix.host.empty())
			ret += "@" + res.prefix.host;
		ret += " ";
	}
	if (!res.command.empty())
		ret += res.command;
	for (std::vector<std::string>::size_type x = 0; x < res.params.size(); x++)
		ret += " " + res.params[x];
	if (!res.trailing.empty())
		ret += " :" + res.trailing;
	// std::cout << "<" << "\"" << ret << "\"" << endl << endl;
	ret += "\r\n";
	return ret;
}
