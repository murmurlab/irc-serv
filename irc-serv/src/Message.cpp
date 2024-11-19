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
	if (!res.command.empty())
		ret += res.command + " ";
	for (int x = 0; x < res.params.size(); x++)
		ret += res.params[x] + " ";
	if (!res.trailing.empty())
		ret += ":" + res.trailing;
        std::cout << "<" << "\"" << ret << "\"" << endl << endl;
        ret += "\r\n";
	return ret;
}
