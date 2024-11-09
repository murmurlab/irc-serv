#include "common.hpp"
#include "irc.hpp"
#include "Server.hpp"

// // static int	sd = 0;
// int					sd;
// int					csd;


// void	socket_dispose(int sig) {
// 	cout << "socket_dispose()" << endl;
// 	close(sd);
// 	close(csd);
// 	std::exit(0);
// }


// // todo: to avoid passing a variadic-size value to read-size argument in read(),
// // give constant-size argument to read-size argument, In this case it will read
// // constant-size byte and will store in the buff1 and if nl is not exist when
// // checked nl in buff1 then part of buff1 that not contains nl is stored in
// // buff2 then read again and store to buff1. Finally, concatenate buff2 with up
// // to the nl part of buff1. buff2 must be read-size bytes in size.
// string get_line_segment(int des) {
// 	static char		buff[BUF_LEN] = { };
// 	static size_t	req_len = BUF_LEN - 1;
// 	static size_t	res_len = read(des, buff, req_len);
// 	static char		*(ab[2]) = {buff, 0};
// 	string			seg;

// 	for (; true;) {
// 		if (res_len == -1)
// 			throw runtime_error("read(): -1");
// 		ab[1] = std::strchr(ab[0], '\n');
// 		// cout << (void *)ab[1] << endl;
// 		if (ab[1])
// 			break ;
// 		if (ab[0] == buff)
// 			throw runtime_error("non-nl");
// 		if (res_len < req_len) {
// 			bzero(buff, BUF_LEN);
// 			req_len = BUF_LEN - 1;
// 			res_len = read(des, buff, req_len);
// 		} else {
// 			// cout << "before1: " << ab[0] << endl;
// 			// cout << "before1: " << buff << endl;
// 			strlcpy(buff, ab[0], (BUF_LEN - 1) - (ab[0] - buff) + 1);
// 			// cout << "before2: " << buff << endl;
// 			bzero(buff + ((BUF_LEN - 1) - (ab[0] - buff)), (ab[0] - buff));
// 			// cout << "before3: " << buff << endl;
// 			req_len = BUF_LEN - ((BUF_LEN - 1) - (ab[0] - buff)) - 1;
// 			res_len = read(des, buff + ((BUF_LEN - 1) - (ab[0] - buff)), req_len);
// 			// cout << "readed: " << buff << endl;
// 		}
// 		ab[0] = buff;
// 		continue ;

// 	}
// 	seg = string(ab[0], (ab[1] - ab[0]) + 1);
// 	ab[0] = ab[1] + 1;
// 	return seg;
// }

// void	parse_prefix() {

// }
// void	parse_command() {
	
// }
// void	parse_params() {
	
// }
// void	parse_unknown() {
	
// }

// void	receive_data(int csd) {
// 	string				line;
// 	std::stringstream	ss;
// 	string				component;
// 	string				trailing;
// 	int					sel_parser;

// 	while (2) {
// 		// sleep(1);
// 		line = get_line_segment(csd);
// 		cout << "> " << line;

// // 		ss.str(line);
// // 		cout << "str: " << ss.str();
// // 		ss >> component;
// // 		cout << "component: " << component << endl;
// // 		cout << "rdbuf: " << ss.rdbuf();
// // 		// cout << "rdbuf all: " << ss.rdbuf();
// // 		std::getline(ss.rdbuf(), trailing);
// // 		cout << "trailing; " << trailing << endl;
// // 		sel_parser = (ss.rdbuf()->sgetc() == ':') ? 0 : 1;
// // 		continue ;
// // 		while (1) {
// // // (ss.rdbuf()->sgetc() == ':')
// // 			switch (sel_parser) {
// // 			case 0: parse_prefix(); sel_parser++;
// // 			case 1: parse_command(); sel_parser++;
// // 			case 2: parse_params();
// // 			default: parse_unknown(); break;
// // 			}
// // 		}
// 		// if (rec == S_Q_CAP_LS)
// 		// 	write(*(int *)csd, S_A_CAP_LS, sizeof(S_A_CAP_LS));
// 		// getchar();
// 	}
	
// 	// size_t				size;
// 	// // sleep(7);
// 	// for (char buff[BUF_LEN] = {}; true; ) {
// 	// 	lseek(csd, 0, SEEK_SET);
// 	// 	if ((size = read(csd, buff, sizeof(buff) - 1)) == -1)
// 	// 		throw runtime_error("read(): " + string(strerror(errno)));
// 	// 	cout << buff;
// 	// 	// cout << "byte: " << size << " data: " << buff << endl;
// 	// 	bzero(buff, BUF_LEN);
// 	// }
// }

// void	*stdin_loop(void *csd) {
// 	string line;
	
// 	while (1) {
// 		// cout << "> ";
// 		getline(std::cin, line);
// 		line += "\r\n";
// 		// cout << "readed: " << line << endl;
// 		write(*(int *)csd, line.c_str(), line.length());
// 	}
// }

// void	test_input(int csd) {
// 	pthread_t th1;
// 	int *a = new int(csd);
// 	pthread_create(&th1, NULL, stdin_loop, a);
// 		// cout << "join: " << endl;
// 	// pthread_join(th1, NULL);
// }

// void	create_socket() {
// 	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
// 	struct sockaddr_in	sin = { };
// 	struct sockaddr_in	sout = { };
// 	socklen_t			sout_len;
// 	sin.sin_family = PF_INET;
// 	sin.sin_port = htons(4445);
// 	sin.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
// 	// IPv4(127, 0, 0, 1)
// 	// cout << inet_addr("127.0.0.1") << endl;
// 	// cout << sin.sin_addr.s_addr << endl;

// 	std::signal(SIGINT, socket_dispose);
// 	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
// 		throw runtime_error("socket(): " + string(strerror(errno)));
// 	// cout << sd << endl;
// 	if (bind(sd, (sockaddr *)&sin, sizeof(sin)) == -1)
// 		throw runtime_error("bind(): " + string(strerror(errno)));
// 	cout << "bind: " << inet_ntoa(sin.sin_addr) << endl;
// 	if (listen(sd, SOMAXCONN) == -1)
// 		throw runtime_error("listen(): " + string(strerror(errno)));
// 	cout << "listen: " << sd << endl;
// 	if ((csd = accept(sd, (sockaddr *)&sout, &sout_len)) == -1)
// 		throw runtime_error("accept(): " + string(strerror(errno)));
// 	cout << "an accept" << endl <<
// 		"\tip: " << inet_ntoa(sout.sin_addr) << endl;
// 	test_input(csd);
// 	try {
// 		receive_data(csd);
// 	} catch (runtime_error& e) {
// 		socket_dispose(0);
// 	}
	
// 	// if (errno)
// 	// 	perror("accept()");
// 	// errno = 0;
// 	socket_dispose(0);
// }

int main(int argc, char const *argv[]) try {
	// if (argc != 3)
	// 	throw (runtime_error(E_ARG));
	// t_irc	irc = {
	// 	.s_q_matcher = {
	// 		[Q_CAP_LS_302] = S_Q_CAP_LS_302,

	// 	}
	// };
	Server s1("127.0.0.1", 4445);
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
	return -1;
}
