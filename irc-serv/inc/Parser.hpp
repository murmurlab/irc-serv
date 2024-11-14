#if !defined(PARSER_HPP)
# define PARSER_HPP

# include <list>
# include <sstream>
# include <string>
# include <typeinfo>

# define BUF_LEN	512

using std::string;
using std::exception;

class IRC_MsgIncomplate: public exception {
	public:
		IRC_MsgIncomplate() : msg_(typeid(*this).name()) {}

		/** Constructor (C strings).
		 *  @param message	The error message.
		 *  				the function is explicit to block implicitly cast
		 */
		explicit IRC_MsgIncomplate(const char* message) 
			: msg_(string(message)) {}
		explicit IRC_MsgIncomplate(const std::string& message)
			: msg_(string(message)) {}

		virtual ~IRC_MsgIncomplate() throw() {}
		virtual const char* what() const throw() {
			return msg_.c_str();
		}
	protected:
		std::string msg_;
};

class	GetLineSegment {
	char						_buff[BUF_LEN];
	size_t						_req_len;
	size_t						_res_len;
	char						*(_ab[2]);
public:
	int							_desc;
	string						_seg;
	void						get_line_segment();
								GetLineSegment(int desc_);
};

typedef enum e_component {
	PARSE_PREFIX,
	PARSE_COMMND,
	PARSE_PARAMS,
	PARSE_UNKNWN
} e_component;

typedef struct Parser
{
	e_component					_sel_parser;
	GetLineSegment				_gls;
	std::stringstream			_ss;
	typedef IRC_MsgIncomplate	IRC_MsgIncomplate;

public:
	void						_parse();
	string						prefix;
	string						command;
	std::list<string>			params;
								Parser(int desc_of_gls);
}	Parser;


#endif // PARSER_HPP
