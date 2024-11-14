#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>

# include "Server.hpp"

using std::string;

typedef struct Client {
private:
	Parser				_parser;
public:
	void				on_data();
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						// Client(Client const &copy);
						Client(int desc, struct sockaddr_in addr);
}	Client;


#endif // CLIENT_HPP
