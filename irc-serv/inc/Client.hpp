#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>

# include "Lexer.hpp"
# include "Evaluator.hpp"

using std::string;

class	Lexer;

class Client {
private:
	// std::list<IRC_msg>	&_msgs;
public:
	Lexer				_lexer;
	Evaluator			_evaluator;

	// int					authenticate();
	bool				is_authz;
	string				username;
	string				nickname;
	
	void				on_data();
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						// Client(Client const &copy);
						Client(int desc, struct sockaddr_in addr);
};


#endif // CLIENT_HPP
