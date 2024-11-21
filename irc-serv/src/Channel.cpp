#include "Channel.hpp"

Channel::Channel(Client &client, string &name): name(name) {
	clients.push_back(&client);
	// ops.push_back(&client);
	invite_only = false;
	topic_protect = false;
	limitted = false;
	limit = 0;
}
