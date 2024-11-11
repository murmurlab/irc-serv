#if !defined(SERVER_HPP)
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>

# include <pthread.h>

# include <iostream>
# include <stdexcept>
# include <list>

// # include <cstddef>
# include <cstring>
# include <csignal>
# include <cerrno>
# include <cstdlib>

# include "Parser.hpp"

using std::endl;
using std::string;
using std::runtime_error;
using std::cout;

typedef short	t_port;

typedef struct s_passive {
	int					sd;
	struct sockaddr_in	addr;
	socklen_t			len;
}	t_passive;

typedef struct Server {
private:
	struct addrinfo			_addrinfo;
	
	struct sockaddr_in		_listen_addr_in;
	int						_listen_sd;
	
	t_passive				_accept_sock;
	std::list<t_passive>	_actives_addr_in;

	void					_listen(in_addr_t host, in_port_t port);
public:
	Server(string host, t_port port);
} Server;

#endif // SERVER_HPP
