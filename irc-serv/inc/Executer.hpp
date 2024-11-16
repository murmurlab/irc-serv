#if !defined(EXECUTER_HPP)
# define EXECUTER_HPP

# include <string>
# include <iostream>

using std::string;
using std::endl;
using std::cout;

class	Parser;

class Executer {
	Parser	&_parser;
	void	_match();
public:
			Executer(Parser &parser);
	void	execute();
};

#endif // EXECUTER_HPP
