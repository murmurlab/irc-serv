#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>

# include "Server.hpp"

using std::string;

typedef struct Client {
private:

public:
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						Client();
}	Client;


#endif // CLIENT_HPP
