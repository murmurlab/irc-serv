#include "Client.hpp"

Client::Client(int desc_, struct sockaddr_in addr_):
	desc(desc_), addr(addr_), len(sizeof(addr_)), _parser(desc_),
	_executer(_parser) {
}

// Client::Client(Client const &copy):
// 	desc(copy.desc), addr(copy.addr), len(sizeof(copy.addr)),
// 	_parser(copy.desc) {
// }

void	Client::on_data() {
	_executer.execute();
}

