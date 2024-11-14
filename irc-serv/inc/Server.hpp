#if !defined(SERVER_HPP)
# define SERVER_HPP

# include <poll.h>
// # include <sys/poll.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>

# include <pthread.h>

# include <iostream>
# include <stdexcept>
# include <list>
# include <vector>

// # include <cstddef>
# include <cstring>
# include <csignal>
# include <cerrno>
# include <cstdlib>

# include "Parser.hpp"
# include "Client.hpp"

using std::endl;
using std::string;
using std::runtime_error;
using std::cout;

# define TIMEOUT_INF		-1
# define TIMEOUT_POLLING	TIMEOUT_INF

typedef short	t_port;

// typedef struct s_passive {
// 	int					sd;
// 	struct sockaddr_in	addr;
// 	socklen_t			len;
// }	t_passive;

typedef struct Server {
private:
	struct addrinfo				_addrinfo;

	std::vector<struct pollfd>	_vec_pollfd;
	
	int							_listen_desc;
	struct sockaddr_in			_listen_addr;
	socklen_t					_listen_len;
	
	std::vector<class Client *>	_accepts;

	void						_update_pollfd();
	void						_add_accept();
	void						_listen(in_addr_t host, in_port_t port);
public:
	Server(string host, t_port port);
	~Server();
} Server;

#endif // SERVER_HPP