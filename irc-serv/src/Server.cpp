#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Instruction.hpp"
#include "Lexer.hpp"
#include "Message.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ios>
#include <netinet/in.h>
#include <string>
#include <sys/_types/_socklen_t.h>
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
	int					*option_value;
	
	std::memset(&_listen_addr, 0, sizeof(_listen_addr));
	// std::memset(&_active, 0, sizeof(_active));
	_listen_addr.sin_family = PF_INET;
	_listen_addr.sin_port = port;
	_listen_addr.sin_addr.s_addr = host;
	// _listen_addr.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << _listen_addr.sin_addr.s_addr << endl;

	// std::signal(SIGINT, server_sigint);
	if ((_listen_desc = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		throw runtime_error("socket(): " + string(strerror(errno)));
	if (setsockopt(_listen_desc, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) < 0 )
		perror("setsockopt(): ");
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
	struct sockaddr_in	addr;
	socklen_t			len;
	int					desc;
	// Client			new_Client;
	struct pollfd		new_pollfd;
	
	std::memset(&addr, 0, sizeof(addr));
	desc = accept(
			_listen_desc, (sockaddr *)&addr, &len
		);
	if (desc == -1)
		throw runtime_error("accept(): " + string(strerror(errno)));
	if (fcntl(desc, F_SETFL, O_NONBLOCK) == -1)
		throw runtime_error("fcntl(): " + string(strerror(errno)));
	// if (setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) < 0 )
	// 	perror("setsockopt(): ");
	cout	<< "[LISTENER] " << "POLLRDNORM " 
			<< inet_ntoa(addr.sin_addr) << endl;
	test_input(desc);
	new_pollfd.fd = desc;
	new_pollfd.events = POLLRDNORM | POLLHUP;
	_accepts.push_back(new Client(desc, addr, *this));
	_vec_pollfd.push_back(new_pollfd);
	// _update_pollfd();
}

static void	write_data(int desc, string const &line) {
	write(desc, line.c_str(), line.length());
	// cout << (int)line.c_str()[line.length() - 1] << endl;
	// cout << (int)line.c_str()[line.length() - 2] << endl;
	// cout << "write_data: " << "[" << line << "]" << endl;
}

bool	Server::_resolveOne(Client &receiver) {
	// responses -> jobs
	if (receiver._evaluator.promises.empty())
		return false;
	Instruction		&res_msg = receiver._evaluator.promises.front();
	cout << "promise: " << res_msg.opr << "comm: " << res_msg.msg.command << "[" << endl;
	switch (res_msg.opr) {
		case EMIT:
			break;
		case SEND:
			cout << "SEND: ";
			// if (res_msg.prefix.u)
			write_data(receiver.desc, Message::_serialize(res_msg.msg));
			// for (std::vector<class Client *>::size_type i = 0; i < _accepts.size(); i++) {
			// 	if (_accepts[i]->username == *res_msg.prefix.user) {
			// 		write_data(receiver.desc, res_raw);
			// }
			break;
		case VOID:
			cout << "VOID: " << endl;
			break;
	}
	cout << "]" << "promised!" << endl;
	receiver._evaluator.promises.pop_front();
	return true;
}

bool	Server::authorize(string const &pass) {
	return pass == this->pass;
}

Channel	*Server::getChannelByName(string &channel) {
	for (std::vector<Channel *>::size_type i = 0; i < _channels.size(); i++) {
		if (_channels[i]->name == channel)
			return _channels[i];
	}
	return NULL;
}

Client	*Server::getClientByNick(string const &x) {
	for (std::vector<Client *>::size_type i = 0; i < _accepts.size(); i++) {
		if (_accepts[i]->nickname == x)
			return _accepts[i];
	}
	return NULL;
}

e_err_reply	Server::join_ch(Client &client, string &channel, string &key) {
	Channel		*ch;
	
	
	ch = getChannelByName(channel);
	if (channel[0] != '&')
		return ERR_NOSUCHCHANNEL;
	if (ch == NULL) {
		ch = new Channel(client, channel);
		_channels.push_back(ch);
		client.chs.push_back(ch);
	}
	if (ch->limitted && ch->clients.size() >= ch->limit)
		return ERR_CHANNELISFULL;
	if (ch->invite_only)
		return ERR_INVITEONLYCHAN;
	if (!key.empty() && ch->pass != key)
		return ERR_BADCHANNELKEY;
	ch->clients.push_back(&client);
	client.chs.push_back(ch);
	return SUCCESS;
}

Server::~Server() {
	for(; !_accepts.empty(); _accepts.pop_back())
		delete _accepts.back();
}

Server::Server(string host, t_port port, string pass): _listen_len(sizeof(_listen_addr)), 
	_listen_desc(-1), pass(pass) {
	nfds_t	poll_ed;

	_listen(inet_addr(host.c_str()), htons(port));
	while (true) {
		// cout << "while turue" << endl;
		poll_ed = poll(_vec_pollfd.data(), (nfds_t)_vec_pollfd.size(), TIMEOUT_POLLING);
		if (poll_ed < 0)
			throw runtime_error("poll(): " + string(strerror(errno)));
		// cout << "poll_ed: " << poll_ed << endl;
		// continue ;
		// cout << "0: " << (void *)(_vec_pollfd[0].revents) << endl;
		if (_vec_pollfd[0].revents & POLLRDNORM) {
			// cout << "POLLRDNORM in listener" << endl;
			// _vec_pollfd[0].revents = 0;
			_add_accept();
		}
		for (nfds_t i = 1; i < (nfds_t)_vec_pollfd.size(); i++) {
			// cout << i << ": " << (void *)(_vec_pollfd[i].revents) << endl;
			if (_vec_pollfd[i].revents & POLLHUP) {
				cout << " disconnected " << i - 1 << endl;
				_vec_pollfd.erase(_vec_pollfd.begin() + i);
				_accepts.erase(_accepts.begin() + i);
			}
			else if (_vec_pollfd[i].revents & POLLRDNORM) {
				// cout << "POLLRDNORM in:" << i - 1 << endl;
				// _vec_pollfd[i].revents = 0;
				// Parser	p1(_vec_pollfd[i].fd);
				_accepts[i - 1]->on_data();
				// std::cout << "/* message */" << _accepts[i - 1]->desc << std::endl;
				while(_resolveOne(*_accepts[i - 1]));
				// _respondAll(_accepts[i - 1]);
			}
		}
	}	
	// if (errno)
	// 	perror("accept()");
	// errno = 0;
	// server_sigint(0);
	// Parser	parser(_accept_sock.sd);
}

