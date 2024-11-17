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
	void				_match(std::list<Message> &msgs);
	
public:
	std::list<Message>	responses;
	string				_serialize();
						Executer(Parser &parser, Client &me_);
	void				execute();
};

#endif // EXECUTER_HPP
