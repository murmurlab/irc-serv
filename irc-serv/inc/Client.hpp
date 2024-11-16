#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>

# include "Parser.hpp"
# include "Executer.hpp"

using std::string;

class	Parser;
// class	Executer;

typedef struct Client {
private:
	Parser				_parser;
	Executer			_executer;
	// std::list<IRC_msg>	&_msgs;
public:
	void				on_data();
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						// Client(Client const &copy);
						Client(int desc, struct sockaddr_in addr);
}	Client;


#endif // CLIENT_HPP
