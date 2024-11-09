#include "./inc/Server.hpp"

static string get_line_segment(int des) {
	static char		buff[BUF_LEN] = { };
	static size_t	req_len = BUF_LEN - 1;
	static size_t	res_len = read(des, buff, req_len);
	static char		*(ab[2]) = {buff, 0};
	string			seg;

	for (; true;) {
		if (res_len == -1)
			throw runtime_error("read(): -1");
		ab[1] = std::strchr(ab[0], '\n');
		// cout << buff << endl;
		// cout << "nl: " << (void *)ab[1] << endl;
		if (ab[1])
			break ;
		if (ab[0] == buff)
			throw runtime_error("non-nl");
		if (res_len < req_len) {
			bzero(buff, BUF_LEN);
			req_len = BUF_LEN - 1;
			res_len = read(des, buff, req_len);
		} else {
			// cout << "before1: " << ab[0] << endl;
			// cout << "before1: " << buff << endl;
			strlcpy(buff, ab[0], (BUF_LEN - 1) - (ab[0] - buff) + 1);
			// cout << "before2: " << buff << endl;
			bzero(buff + ((BUF_LEN - 1) - (ab[0] - buff)), (ab[0] - buff));
			// cout << "before3: " << buff << endl;
			req_len = BUF_LEN - ((BUF_LEN - 1) - (ab[0] - buff)) - 1;
			res_len = read(des, buff + ((BUF_LEN - 1) - (ab[0] - buff)), req_len);
			// cout << "readed: " << buff << endl;
		}
		ab[0] = buff;
		continue ;

	}
	seg = string(ab[0], (ab[1] - ab[0]) + 1);
	ab[0] = ab[1] + 1;
	return seg;
}

static void	receive_data(int csd) {
	string rec;
	
	while (2) {
		// sleep(1);
		rec = get_line_segment(csd);
		cout << "> " << rec;
		// getchar();
	}
	
	// size_t				size;
	// // sleep(7);
	// for (char buff[BUF_LEN] = {}; true; ) {
	// 	lseek(csd, 0, SEEK_SET);
	// 	if ((size = read(csd, buff, sizeof(buff) - 1)) == -1)
	// 		throw runtime_error("read(): " + string(strerror(errno)));
	// 	cout << buff;
	// 	// cout << "byte: " << size << " data: " << buff << endl;
	// 	bzero(buff, BUF_LEN);
	// }
}


Server::Server(string addr, short port)
{
	memset(&this->sock, 0, sizeof(this->sock));
	this->create_socket(addr, port);
}

Server::~Server()
{
	this->socket_dispose(0);
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




int	Server::create_socket(string addr, short port)
{
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	struct sockaddr_in	sin = { };
	struct sockaddr_in	sout = { };
	socklen_t			sout_len;
	sin.sin_family = PF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr.c_str());
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << sin.sin_addr.s_addr << endl;

	// std::signal(SIGINT, socket_dispose);
	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		throw runtime_error("socket(): " + string(strerror(errno)));
	// cout << sd << endl;
	if (bind(sd, (sockaddr *)&sin, sizeof(sin)) == -1)
		throw runtime_error("bind(): " + string(strerror(errno)));
	cout << "bind: " << inet_ntoa(sin.sin_addr) << endl;
	if (listen(sd, SOMAXCONN) == -1)
		throw runtime_error("listen(): " + string(strerror(errno)));
	cout << "listen: " << sd << endl;
	if ((csd = accept(sd, (sockaddr *)&sout, &sout_len)) == -1)
		throw runtime_error("accept(): " + string(strerror(errno)));
	cout << "an accept" << endl <<
		"\tip: " << inet_ntoa(sout.sin_addr) << endl;
	test_input(csd);
	try {
		receive_data(csd);
	} catch (runtime_error& e) {
		socket_dispose(0);
	}
	
	// if (errno)
	// 	perror("accept()");
	// errno = 0;
	socket_dispose(0);
	return 0;
}

void	Server::socket_dispose(int sig)
{
	cout << "socket_dispose()" << endl;
	// close(sd);
	close(csd);
	std::exit(0);
}

