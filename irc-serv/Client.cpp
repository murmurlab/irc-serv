#include "./inc/Client.hpp"

Client::Client(in_addr_t addr, in_port_t port)
{
 memset(&this->sock, 0, sizeof(this->sock));
}

Client::~Client()
{
	this->socket_dispose(0);
}

void	Client::socket_dispose(int sig)
{
	cout << "socket_dispose()" << endl;
	// close(sd);
	std::exit(0);
}
