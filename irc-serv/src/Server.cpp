#include "Server.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include <cstddef>
#include <cstring>
#include <sys/fcntl.h>
#include <sys/poll.h>

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
	struct pollfd		new_pollfd;
	
	std::memset(&_listen_addr, 0, sizeof(_listen_addr));
	// std::memset(&_active, 0, sizeof(_active));
	_listen_addr.sin_family = PF_INET;
	_listen_addr.sin_port = port;
	_listen_addr.sin_addr.s_addr = host;
	// _listen_addr.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << _listen_addr.sin_addr.s_addr << endl;

	std::signal(SIGINT, server_sigint);
	if ((_listen_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		throw runtime_error("socket(): " + string(strerror(errno)));
	// cout << sd << endl;
	if (bind(_listen_desc, (sockaddr *)&_listen_addr, sizeof(_listen_addr)) == -1)
		throw runtime_error("bind(): " + string(strerror(errno)));
	cout << "bind: " << inet_ntoa(_listen_addr.sin_addr) << endl;
	if (listen(_listen_desc, SOMAXCONN) == -1)
		throw runtime_error("listen(): " + string(strerror(errno)));
	cout << "listen: " << _listen_desc << endl;
	new_pollfd.fd = _listen_desc;
	new_pollfd.events = POLLRDNORM;
	new_pollfd.revents = 0;
	_vec_pollfd.push_back(new_pollfd);
}

void	Server::_update_pollfd() {
	// struct pollfd				*tmp_pollfd;
	// std::vector<struct pollfd>	vec_pollfd;

	// tmp_pollfd = new struct pollfd[_poll_len + 1];
	// std::memset(tmp_pollfd, 0, sizeof(struct pollfd) * _poll_len + 1);
	// std::memcpy(tmp_pollfd, _poll, sizeof(struct pollfd) * _poll_len);
	// delete [] _poll;
	// _poll = tmp_pollfd;
}

void	Server::_add_accept() {
	Client			new_Client;
	struct pollfd	new_pollfd;
	
	new_Client.desc = accept(
			_listen_desc, (sockaddr *)&new_Client.addr, &new_Client.len
		);
	cout	<< "[LISTENER] " << "POLLRDNORM " 
			<< inet_ntoa(new_Client.addr.sin_addr) << endl;
	// test_input(new_Client.desc);
	if (new_Client.desc == -1)
		throw runtime_error("accept(): " + string(strerror(errno)));
	new_pollfd.fd = new_Client.desc;
	new_pollfd.events = POLLRDNORM;
	fcntl(new_Client.desc, F_SETFL, O_NONBLOCK);
	_accepts.push_back(new_Client);
	_vec_pollfd.push_back(new_pollfd);
	new_Client.~Client();
	// _update_pollfd();
}

Server::Server(string host, t_port port): _listen_len(sizeof(_listen_addr)), 
	_listen_desc(-1) {
	nfds_t	poll_ed;

	_listen(inet_addr(host.c_str()), htons(port));
	while (true) {
		cout << "while turue" << endl;
		poll_ed = poll(_vec_pollfd.data(), (nfds_t)_vec_pollfd.size(), TIMEOUT_POLLING);
		if (poll_ed < 0)
			throw runtime_error("poll(): " + string(strerror(errno)));
		if (_vec_pollfd[0].revents == POLLRDNORM)
			_add_accept();
		for (nfds_t i = 1; i < (nfds_t)_vec_pollfd.size(); i++) {
			if (_vec_pollfd[i].revents == POLLRDNORM)
				Parser	p1(_vec_pollfd[i].fd);
		}
	}	
	// if (errno)
	// 	perror("accept()");
	// errno = 0;
	server_sigint(0);
	// Parser	parser(_accept_sock.sd);
}

