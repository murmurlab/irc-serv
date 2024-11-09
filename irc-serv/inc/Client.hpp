#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "common.hpp"
#include "irc.hpp"

class Client
{
	private:
		int sd;
		const static size_t P_IP;
		struct sockaddr_in	sock;
		socklen_t			sout_len;
	public:
		Client(in_addr_t addr, in_port_t port);
		~Client();
		void	socket_dispose(int sig);
};

#endif // CLIENT_HPP
