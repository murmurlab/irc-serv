#if !defined(EXECUTER_HPP)
# define EXECUTER_HPP

# include <string>
# include <iostream>
# include <list>

// #include "Message.hpp"

using std::string;
using std::endl;
using std::cout;

class	Parser;
class	Message;
class	Client;

class Executer {
	Client				&_me;
	Parser				&_parser;
	void				_matchAll(std::list<Message> &msgs);
	int					_matchOne(std::list<Message> &msgs);
public:
	std::list<Message>	responses;
	string				_serialize(Message const &res);
						Executer(Parser &parser, Client &me_);
	void				execute();
};

# include "Client.hpp"

#endif // EXECUTER_HPP
