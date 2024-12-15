#include "Client.hpp"
#include "Evaluator.hpp"
#include "Server.hpp"

Client::Client(int desc_, struct sockaddr_in addr_, Server &server_):
	_lexer(desc_), _server(server_), _evaluator(_lexer, *this),
	authorized(false), desc(desc_), addr(addr_), len(sizeof(addr_))
	{
}

// Client::Client(Client const &copy):
// 	desc(copy.desc), addr(copy.addr), len(sizeof(copy.addr)),
// 	_parser(copy.desc) {
// }

Client::~Client() {
	cout << "Client::~Client()" << endl;
	// for (std::vector<Channel *>::size_type i = 0; i < chs.size(); i++)
	// 	_server.leave_ch(*this, *chs[i]);
	for (; !chs.empty();) {
		_server.leave_ch(*this, *chs.back());
	}
}

void	Client::on_data() {
	try {
		_lexer.lex();
	} catch (IRC_MsgIncomplate& e) {
		// cout << e.what() << ": Client::on_data(): IRC_MsgIncomplate" << endl<< endl;
	}
	_evaluator.eval();
}
