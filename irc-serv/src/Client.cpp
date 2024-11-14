#include "Client.hpp"
#include "Parser.hpp"
#include <cstring>

Client::Client(int desc_, struct sockaddr_in addr_):
	desc(desc_), addr(addr_), len(sizeof(addr_)), _parser(desc_) {
}

// Client::Client(Client const &copy):
// 	desc(copy.desc), addr(copy.addr), len(sizeof(copy.addr)),
// 	_parser(copy.desc) {
// }

void	Client::on_data() {
	try {
		_parser._parse();
		// execute command
	} catch (IRC_MsgIncomplate& e) {
		cout << e.what() << ": Client::on_data(): IRC_MsgIncomplate" << endl;
	}
}

