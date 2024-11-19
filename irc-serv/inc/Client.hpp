#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>

# include "Lexer.hpp"
# include "Evaluator.hpp"

using std::string;

class	Lexer;
class	Server;

class Client {
private:
	// std::list<IRC_msg>	&_msgs;
	friend class Evaluator;
	Lexer				_lexer;
	Server				&_server;
public:
	Evaluator			_evaluator;

	bool				authorized;
	string				username;
	string				nickname;
	
	void				on_data();
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						// Client(Client const &copy);
						Client(int desc, struct sockaddr_in addr, Server &server);
};


#endif // CLIENT_HPP
