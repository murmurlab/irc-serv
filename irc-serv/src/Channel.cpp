#include "Channel.hpp"

Channel::Channel(ChMember &op, string &name): name(name) {
	members.push_back(op);
	// ops.push_back(&client);
	this->flags.i = false;
	this->flags.t = false;
	this->flags.l = false;
	limit = 0;
}
