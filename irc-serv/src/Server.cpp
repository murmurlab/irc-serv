#include "Server.hpp"
#include "ChMember.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Evaluator.hpp"
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

// static void	server_sigint(int sig) {
// 	(void)sig;
// 	cout << "server_sigint()" << endl;
// 	// close(sd);
// 	// close(csd);
// 	std::exit(0);
// }

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
	// const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
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
	// yellow color
	cout << "\033[33m" << ">" << desc << " " << string(line.begin(), line.end() - 2) << "\033[0m" << endl;
	// cout << ">" << desc << " " << string(line.begin(), line.end() - 2) << endl;
}

bool	Server::_resolveOne(Client &receiver) {
	// responses -> jobs
	if (receiver._evaluator.promises.empty())
		return false;
	Instruction		&res_msg = receiver._evaluator.promises.front();
	// cout << "promise: " << res_msg.opr << "comm: " << res_msg.msg.command << "[" << endl;
	switch (res_msg.opr) {
		case EMIT:
			for (std::vector<class Client *>::size_type i = 0; i < res_msg.clients.size(); i++) {
				write_data(res_msg.clients[i]->desc, Message::_serialize(res_msg.msg));
			}
			break;
		case SEND:
			// cout << "SEND: ";
			// if (res_msg.prefix.u)
			write_data(receiver.desc, Message::_serialize(res_msg.msg));
			// for (std::vector<class Client *>::size_type i = 0; i < _accepts.size(); i++) {
			// 	if (_accepts[i]->username == *res_msg.prefix.user) {
			// 		write_data(receiver.desc, res_raw);
			// }
			break;
		case VOID:
			// cout << "VOID: " << endl;
			break;
	}
	// cout << "]" << "promised!" << endl;
	receiver._evaluator.promises.pop_front();
	return true;
}

bool	Server::authorize(string const &pass) {
	return pass == this->pass;
}

void	Server::leave_ch(Client &client, Channel &ch) {
	// list of channels members
	for (std::vector<Channel *>::size_type i = 0; i < client.chs.size(); i++) {
		if (client.chs[i] == &ch) {
			client.chs.erase(client.chs.begin() + i);
			break;
		}
	}
	for (std::vector<ChMember>::size_type i = 0; i < ch.members.size(); i++) {
		if (ch.members[i].client == &client) {
			ch.members.erase(ch.members.begin() + i);
			break;
		}
	}
	// ch.members.size()
	// cout << "member size mentioned channel: " << ch.members.size() << " " << ch.name << endl;
	if (ch.members.empty()) {
		for (std::vector<Channel *>::size_type i = 0; i < _channels.size(); i++) {
			if (_channels[i] == &ch) {
				_channels.erase(_channels.begin() + i);
				cout << "channel " << ch.name << " deleted" << endl;
				delete &ch;
				break;
			}
		}
	}
}

ChMember	*Server::getMemberByName(Channel &ch, string &nick) {
	for (std::vector<ChMember>::size_type i = 0; i < ch.members.size(); i++) {
		if (ch.members[i].client->nickname == nick)
			return &ch.members[i];
	}
	return NULL;
}

Channel	*Server::getChannelByName(string &channel) {
	for (std::vector<Channel *>::size_type i = 0; i < _channels.size(); i++) {
		if (_channels[i]->name == channel)
			return _channels[i];
	}
	return NULL;
}

Client	*Server::getClientByNick(string const &x) {
	if (x.empty())
		return NULL;
	for (std::vector<Client *>::size_type i = 0; i < _accepts.size(); i++) {
		if (_accepts[i]->nickname == x)
			return _accepts[i];
	}
	return NULL;
}

Channel	&Server::join_ch(Client &client, string &channel, string &key) {
	Channel		*ch;
	ChMember	memb(&client);
	
	ch = getChannelByName(channel);
	if (channel[0] != '&')
		throw ERR_NOSUCHCHANNEL;
	if (ch == NULL) {
		memb.op = true;
		ch = new Channel(memb, channel);
		_channels.push_back(ch);
		client.chs.push_back(ch);
		return *ch;
	}
	for (std::vector<ChMember>::size_type i = 0; i < ch->members.size(); i++) {
		if (ch->members[i].client == &client)
			throw SUCCESS;
	}
	if (ch->flags.l && ch->members.size() >= ch->limit)
		throw ERR_CHANNELISFULL;
	if (ch->flags.i)
		throw ERR_INVITEONLYCHAN;
	if ((ch->pass != key) && !ch->pass.empty()) {
		// if (!key.empty())
		throw ERR_BADCHANNELKEY;
	}
	ch->members.push_back(memb);
	client.chs.push_back(ch);
	return *ch;
}

Server::~Server() {
	for(; !_accepts.empty(); _accepts.pop_back())
		delete _accepts.back();
}

Server::Server(string host, t_port port, string pass): _listen_desc(-1), _listen_len(sizeof(_listen_addr)), pass(pass) {
	nfds_t	poll_ed;

	_listen(inet_addr(host.c_str()), htons(port));
	while (true) {
		poll_ed = poll(_vec_pollfd.data(), (nfds_t)_vec_pollfd.size(), TIMEOUT_POLLING);
		cout << "poll" << endl;
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
			cout << "checking: " << i << endl;
			// cout << i << ": " << (void *)(_vec_pollfd[i].revents) << endl;
			if (_vec_pollfd[i].revents & POLLHUP) {
				cout << " POLLHUP index: " << i << endl;
				_vec_pollfd.erase(_vec_pollfd.begin() + i);
				delete _accepts[i - 1];
				_accepts.erase(_accepts.begin() + (i - 1));
				// print pollfds
			}
			else if (_vec_pollfd[i].revents & POLLRDNORM) {
				// cout << "POLLRDNORM in:" << i - 1 << endl;
				// _vec_pollfd[i].revents = 0;
				// Parser	p1(_vec_pollfd[i].fd);
				cout << "POLLRDNORM index: " << i << " on_data: " << _accepts[i - 1]->desc << endl;
				try {
					_accepts[i - 1]->on_data();
				} catch (runtime_error &e) {
					cout << e.what() << "======" << endl;
					_vec_pollfd.erase(_vec_pollfd.begin() + i);
					delete _accepts[i - 1];
					_accepts.erase(_accepts.begin() + (i - 1));
					i--;
					continue ;
				}
				while(_resolveOne(*_accepts[i - 1]));
				// _respondAll(_accepts[i - 1]);
			}
			for (std::vector<struct pollfd>::size_type i = 0; i < _vec_pollfd.size(); i++)
				cout << i << ": " << _vec_pollfd[i].fd << endl;
		}
	}
	// if (errno)
	// 	perror("accept()");
	// errno = 0;
	// server_sigint(0);
	// Parser	parser(_accept_sock.sd);
}

