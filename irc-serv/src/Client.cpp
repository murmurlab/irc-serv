#include "Client.hpp"
#include "Evaluator.hpp"

Client::Client(int desc_, struct sockaddr_in addr_, Server &server_):
	_lexer(desc_), _server(server_), _evaluator(_lexer, *this), desc(desc_),
	addr(addr_), len(sizeof(addr_)), authorized(false)
	{
}

// Client::Client(Client const &copy):
// 	desc(copy.desc), addr(copy.addr), len(sizeof(copy.addr)),
// 	_parser(copy.desc) {
// }

void	Client::on_data() {
	try {
		_lexer.lex();
	} catch (IRC_MsgIncomplate& e) {
		cout << e.what() << ": Client::on_data(): IRC_MsgIncomplate" << endl<< endl;
	}
	_evaluator.eval();
}
