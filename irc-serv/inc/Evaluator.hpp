#if !defined(EVALUATOR_HPP)
# define EVALUATOR_HPP

#include "Instruction.hpp"
// #include "Server.hpp"
# include <string>
# include <iostream>
# include <list>

// #include "Message.hpp"
typedef enum e_err_reply {
	SUCCESS = 0,
	ERR_NOSUCHCHANNEL = 403,
	ERR_BADCHANNELKEY = 475,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473
}	e_err_reply;

using std::string;
using std::endl;
using std::cout;

class	Lexer;
class	Message;
class	Client;

class Evaluator {
	Client					&_me;
	Lexer					&_lexer;
	
	bool	_check_register(Message &msg);

	void	_CAP_END(Message &msg);
	void	_CAP_LS(Message &msg);
	void	_CAP_REQ(Message &msg);
	void	_CAP_unk(Message &msg);

	void	_CAP(Message &msg);
	void	_PASS(Message &msg);
	void	_QUIT(Message &msg);
	void	_PING(Message &msg);
	void	_JOIN(Message &msg);
	void	_NICK(Message &msg);
	void	_USER(Message &msg);
	void	_MODE(Message &msg);
	void	_TOPIC(Message &msg);
	void	_INVITE(Message &msg);
	void	_KICK(Message &msg);
	void	_PRIVMSG(Message &msg);
	void	_PART(Message &msg);

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
