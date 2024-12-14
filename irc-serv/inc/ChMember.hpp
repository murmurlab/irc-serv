#ifndef CHMEMBER_HPP
# define CHMEMBER_HPP

// # include "Client.hpp"

class Client;

class ChMember {
public:
	Client		*client;
	bool		op;
	
				ChMember(Client *client_, bool op_ = 0);
};

#endif // CHMEMBER_HPP
