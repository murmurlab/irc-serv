// #include "common.hpp"
#include "Server.hpp"
#include "irc.hpp"

int main(int argc, char const *argv[]) try {
	std::istringstream iss;

	if (argc != 3)
		throw (runtime_error(_IRC_E_ARG));
	iss.str(argv[1]);
	int port;
	if (!(iss >> port))
		throw (runtime_error(_IRC_E_ARG));
	if (port < 1024 || port > 65535)
		throw (runtime_error(_IRC_E_ARG));

	cout << "port: " << port << " pass: " << argv[2] << endl;
	Server	s1("10.11.6.11", port, argv[2]);
	s1.~Server();
	// create_socket();
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
	return -1;
}
