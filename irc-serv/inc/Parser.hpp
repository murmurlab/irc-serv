#if !defined(PARSER_HPP)
# define PARSER_HPP

# include <list>
# include <sstream>
# include <string>

using std::string;

typedef struct Parser
{
private:
	size_t				_sel_parser;
	std::stringstream	_ss;
	string				_line;
	int					_csd;

	void				_parse();
public:
	string				prefix;
	string				command;
	std::list<string>	params;

						Parser(int csd);
}	Parser;


#endif // PARSER_HPP
