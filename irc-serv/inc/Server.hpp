#ifndef SERVER_HPP
# define SERVER_HPP

#include "common.hpp"
#include "irc.hpp"

class Server
{
	private:
		int sd;
		int csd;
		const static size_t P_IP;
		struct sockaddr_in	sock;
		socklen_t			sout_len;
	public:
		Server(string addr, short port);
		~Server();
		int		create_socket(string addr, short port);
		void	socket_dispose(int sig);
};

#endif // SERVER_HPP
