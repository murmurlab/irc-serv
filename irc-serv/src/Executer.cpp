#include "Executer.hpp"
#include "Parser.hpp"

Executer::Executer(Parser &parser_): _parser(parser_) {

}

void Executer::_match() {

}

void	Executer::execute() {
	try {
		_parser.parse();
	} catch (IRC_MsgIncomplate& e) {
		cout << e.what() << ": Executer::execute(): IRC_MsgIncomplate" << endl;
	}
	_match();
}
