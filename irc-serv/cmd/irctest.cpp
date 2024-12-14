#include "common.hpp"
#include "irc.hpp"

// void	socket_dispose(int sig) {
// 	cout << "socket_dispose()" << endl;
// 	// close(sd);
// 	std::exit(0);
// }

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
// 		// cout << buff << endl;
// 		// cout << "nl: " << (void *)ab[1] << endl;
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

// void	receive_data(int csd) {
// 	string rec;
	
// 	while (2) {
// 		// sleep(1);
// 		rec = get_line_segment(csd);
// 		cout << "> " << rec;
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

// void	*stdout_loop(void *csd) {
// 	receive_data(*(int *)csd);
// 	return NULL;
// }

// void	test_out(int csd) {
// 	pthread_t th1;
// 	int *a = new int(csd);
// 	pthread_create(&th1, NULL, stdout_loop, a);
// 	// pthread_join(th1, NULL);
// }

// int create_socket(in_addr_t addr, in_port_t port) {
// 	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
// 	struct sockaddr_in	sock = { };
// 	socklen_t			sout_len;
// 	int					sd;
// 	sock.sin_family = PF_INET;
// 	sock.sin_port = port;
// 	sock.sin_addr.s_addr = addr;
// 	// *(int *)"\x7f\x00\x00\x01"
// 	// IPv4(127, 0, 0, 1)
// 	// cout << inet_addr("127.0.0.1") << endl;
// 	// cout << sock.sin_addr.s_addr << endl;

// 	std::signal(SIGINT, socket_dispose);
// 	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
// 		return perror("socket()"), -1;
// 	// if (bind(sd, (sockaddr *)&sock, sizeof(sock)))
// 	// 	return perror("bind()"), -1;
// 	if (connect(sd, (sockaddr *)&sock, sizeof(struct sockaddr_in)) == -1)
// 		return perror("connect()"), -1;
// 	cout << "connect succesfully" << endl;
// 	// pause();
// 	test_out(sd);
// 	// cout << "> ";
// 	for (string line; std::getline(std::cin, line); ) {
// 		// write(sd, line.c_str(), line.length());
// 		// cout << " arst " << endl;
// 		dprintf(sd, "%s\n", line.c_str());
// 		// cout << "> ";
// 	}
// 	// if (listen(sd, SOMAXCONN) == -1)
// 	// 	return perror("listen()"), -1;
// 	// while (accept(sd, (sockaddr *)&sout, &sout_len) == 0);
// 	return 0;
// }

// #include <cstdlib>

int main(int argc, char const *argv[]) try {
	(void)argc;
	(void)argv;
	// if (argc != 3)
	// 	throw runtime_error("least 2 arg");
	// create_socket(inet_addr(argv[1]), htons(std::atoi(argv[2])));
	return 0;
} catch (runtime_error &e) {
	cerr << e.what() << endl;
}
