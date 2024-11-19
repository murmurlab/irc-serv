#include "Client.hpp"

Client::Client(int desc_, struct sockaddr_in addr_):
	_lexer(desc_), _evaluator(_lexer, *this), desc(desc_), addr(addr_),
	len(sizeof(addr_))
	{
}

// Client::Client(Client const &copy):
// 	desc(copy.desc), addr(copy.addr), len(sizeof(copy.addr)),
// 	_parser(copy.desc) {
// }

void	Client::on_data() {
	_evaluator.eval();
}
