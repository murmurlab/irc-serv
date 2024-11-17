#if !defined(PARSER_HPP)
# define PARSER_HPP

# include <GetLineSegment.hpp>
# include "Message.hpp"
# include <list>
# include <string>
# include <sstream>

using std::string;

typedef enum e_component {
	PARSE_PREFIX,
	PARSE_COMMND,
	PARSE_PARAMS,
	PARSE_UNKNWN
} e_component;

typedef enum {
		CAP,
		NICK,
		USER,
		AUTHENTICATE,
		UNKNOWNCMD
}		e_cmd;

class Parser
{
	int							_sel_parser;
	GetLineSegment				_gls;
	std::stringstream			_ss;
	// typedef IRC_MsgIncomplate	IRC_MsgIncomplate;

	Message						*_msg;
	void						_token_commmon();
	void						_token_prefix();
	void						_token_command();
	void						_token_params();
	void						_token_unknown();
	void						_lexer();
	void						print_msg(Message &msg);
	// Message						&_AUTH(Message &req,Message &retRequest);
	// Message						&_CAP(Message &req, Message &retRequest);
public:
	std::list<Message>			msgs;
	void						parse();
								Parser(int desc_of_gls);
};


#endif // PARSER_HPP
