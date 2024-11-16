#include "Message.hpp"
#include <iostream>

Message::Message(): trailing(NULL) {
	// std::cout << "Message construct" << std::endl;
	std::memset(&prefix, 0, sizeof(prefix));
}

Message::~Message() {
	delete trailing;
	delete prefix.nick;
	delete prefix.user;
	delete prefix.host;
}
