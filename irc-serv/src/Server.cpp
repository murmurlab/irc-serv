#include "Server.hpp"
#include "Parser.hpp"

// static int	sd = 0;
// int					sd;
// int					csd;

static void	server_sigint(int sig) {
	cout << "server_sigint()" << endl;
	// close(sd);
	// close(csd);
	std::exit(0);
}

static void	*stdin_loop(void *csd) {
	string line;
	
	while (1) {
		// cout << "> ";
		getline(std::cin, line);
		line += "\r\n";
		// cout << "readed: " << line << endl;
		write(*(int *)csd, line.c_str(), line.length());
	}
}

static void	test_input(int csd) {
	pthread_t th1;
	int *a = new int(csd);
	pthread_create(&th1, NULL, stdin_loop, a);
	// cout << "join: " << endl;
	// pthread_join(th1, NULL);
}

void	Server::_listen(in_addr_t host, in_port_t port) {
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	std::memset(&_listen_addr_in, 0, sizeof(_listen_addr_in));
	// std::memset(&_active, 0, sizeof(_active));
	_listen_addr_in.sin_family = PF_INET;
	_listen_addr_in.sin_port = port;
	_listen_addr_in.sin_addr.s_addr = host;
	// _listen_addr_in.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << _listen_addr_in.sin_addr.s_addr << endl;

	std::signal(SIGINT, server_sigint);
	if ((_listen_sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		throw runtime_error("socket(): " + string(strerror(errno)));
	// cout << sd << endl;
	if (bind(_listen_sd, (sockaddr *)&_listen_addr_in, sizeof(_listen_addr_in)) == -1)
		throw runtime_error("bind(): " + string(strerror(errno)));
	cout << "bind: " << inet_ntoa(_listen_addr_in.sin_addr) << endl;
	if (listen(_listen_sd, SOMAXCONN) == -1)
		throw runtime_error("listen(): " + string(strerror(errno)));
	cout << "listen: " << _listen_sd << endl;
}

Server::Server(string host, t_port port) {
	_listen(inet_addr("127.0.0.1"), htons(port));

	std::memset(&_accept_sock, 0, sizeof(_accept_sock));
	if ((_accept_sock.sd = accept(_listen_sd, (sockaddr *)&_accept_sock.addr, &_accept_sock.len)) == -1)
		throw runtime_error("accept(): " + string(strerror(errno)));

	cout << "an accept" << endl <<
		"\tip: " << inet_ntoa(_accept_sock.addr.sin_addr) << endl;
	test_input(_accept_sock.sd);
	try {
		Parser	p1(_accept_sock.sd);
	} catch (runtime_error& e) {
		server_sigint(0);
	}
	
	// if (errno)
	// 	perror("accept()");
	// errno = 0;
	server_sigint(0);
	Parser	parser(_accept_sock.sd);

}

