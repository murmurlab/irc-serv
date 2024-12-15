#if !defined(CLIENT_HPP)
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>

// # include "Channel.hpp"
# include "Lexer.hpp"
# include "Evaluator.hpp"

using std::string;

class	Channel;
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

	// vector<Channel *>	mod_chs;
	vector<class Channel *>	chs;

	int					authorized;
	string				username;
	string				nickname;
	string				realname;
	void				on_data();
	int					desc;
	struct sockaddr_in	addr;
	socklen_t			len;
						// Client(Client const &copy);
						Client(int desc, struct sockaddr_in addr, Server &server);
						~Client();
};

#endif // CLIENT_HPP
