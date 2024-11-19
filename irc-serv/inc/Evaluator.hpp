#if !defined(EVALUATOR_HPP)
# define EVALUATOR_HPP

#include "Instruction.hpp"
# include <string>
# include <iostream>
# include <list>

// #include "Message.hpp"

using std::string;
using std::endl;
using std::cout;

class	Lexer;
class	Message;
class	Client;

class Evaluator {
	Client					&_me;
	Lexer					&_lexer;
	
	void	_CAP_END(Message &msg);
	void	_CAP_LS(Message &msg);
	void	_CAP_REQ(Message &msg);
	void	_CAP_unk(Message &msg);
	void	_CAP(Message &msg);

	void	_CMD_needarg(Message &msg);
	void	_CMD_unknown(Message &msg);
	int	_evalOne(std::list<Message> &msgs);
public:
	Instruction	&newInstruction();
	std::list<Instruction>	promises;
	string					_serialize(Message const &res);
							Evaluator(Lexer &lexer, Client &me_);
	void					eval();
};

# include "Client.hpp"

#endif // EVALUATOR_HPP
