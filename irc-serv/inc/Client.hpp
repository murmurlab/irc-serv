#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>

# include "Parser.hpp"
# include "Executer.hpp"

using std::string;

class	Parser;

class Client {
private:
	// std::list<IRC_msg>	&_msgs;
public:
	Parser				_parser;
	Executer			_executer;

	string				username;
	void				on_data();
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						// Client(Client const &copy);
						Client(int desc, struct sockaddr_in addr);
};


#endif // CLIENT_HPP
