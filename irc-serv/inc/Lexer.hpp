#if !defined(LEXER_HPP)
# define LEXER_HPP

# include <GetLineSegment.hpp>
# include "Message.hpp"
# include <list>
# include <string>
# include <sstream>

using std::string;

typedef enum e_component {
	LEX_PREFIX,
	LEX_COMMND,
	LEX_PARAMS,
	LEX_UNKNWN
} e_component;

typedef enum {
		CAP,
		NICK,
		USER,
		AUTHENTICATE,
		UNKNOWNCMD
}		e_cmd;

class Lexer
{
	int							_sel_lexer;
	GetLineSegment				_gls;
	std::stringstream			_ss;
	// typedef IRC_MsgIncomplate	IRC_MsgIncomplate;

	Message						*_msg;
	void						_token_commmon();
	void						_token_prefix();
	void						_token_command();
	void						_token_params();
	void						_token_unknown();
	void						_tokenize();
	void						print_msg(Message &msg);
	// Message						&_AUTH(Message &req,Message &retRequest);
	// Message						&_CAP(Message &req, Message &retRequest);
public:
	std::list<Message>			msgs;
	void						lex();
								Lexer(int desc_of_gls);
};


#endif // LEXER_HPP
