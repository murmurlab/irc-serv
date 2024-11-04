#include "common.hpp"
#include "irc.hpp"

void	socket_dispose(int sig) {
	cout << "socket_dispose()" << endl;
	// close(sd);
	std::exit(0);
}

int create_socket(in_addr_t addr, in_port_t port) {
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	struct sockaddr_in	sock = { };
	socklen_t			sout_len;
	int					sd;
	sock.sin_family = PF_INET;
	sock.sin_port = port;
	sock.sin_addr.s_addr = addr;
	// *(int *)"\x7f\x00\x00\x01"
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << sock.sin_addr.s_addr << endl;

	std::signal(SIGINT, socket_dispose);
	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		return perror("socket()"), -1;
	// if (bind(sd, (sockaddr *)&sock, sizeof(sock)))
	// 	return perror("bind()"), -1;
	if (connect(sd, (sockaddr *)&sock, sizeof(struct sockaddr_in)) == -1)
		return perror("connect()"), -1;
	cout << "connect succesfully" << endl;
	// pause();
	for (string line; std::getline(std::cin, line); ) {
		// write(sd, line.c_str(), line.length());
		// cout << " arst " << endl;
		dprintf(sd, "%s\n", line.c_str());
	}
	// if (listen(sd, SOMAXCONN) == -1)
	// 	return perror("listen()"), -1;
	// while (accept(sd, (sockaddr *)&sout, &sout_len) == 0);
	return 0;
}

#include <cstdlib>

int main(int argc, char const *argv[]) try {
	if (argc != 3)
		throw runtime_error("least 2 arg");
	create_socket(inet_addr(argv[1]), htons(std::atoi(argv[2])));
	return 0;
} catch (runtime_error &e) {
	cerr << e.what() << endl;
}
