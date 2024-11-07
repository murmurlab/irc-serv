#include "common.hpp"
#include "irc.hpp"

// static int	sd = 0;

void	socket_dispose(int sig) {
	cout << "socket_dispose()" << endl;
	// close(sd);
	std::exit(0);
}

string get_line_segment(int des) {
	static char		buff[BUF_LEN] = { };
	static size_t	req_len = BUF_LEN - 1;
	static size_t	res_len = read(des, buff, req_len);
	static char		*(ab[2]) = {buff, 0};
	string			seg;

	for (; true;) {
		if (res_len == -1)
			throw runtime_error("read(): -1");
		ab[1] = std::strchr(ab[0], '\n');
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

// char	*get_line_segment(int des) {
// 	static char		buff[BUF_LEN] = { };
// 	static size_t	req_len = BUF_LEN - 1;
// 	static size_t	res_len = read(des, buff, req_len);
// 	static char		*(ab[2]) = {buff, 0};
// 	char			*seg;

// 	for (; true;) {
// 		if (res_len == -1)
// 			throw runtime_error("read(): -1");
// 		ab[1] = std::strchr(ab[0], '\n');
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
// 	seg = (char *)std::calloc((ab[1] - ab[0]) + 1, 1);
// 	std::memcpy(seg, ab[0], (ab[1] - ab[0]) + 1);
// 	ab[0] = ab[1] + 1;
// 	return seg;
// }


void	receive_data(int csd) {
	string rec;
	
	while (2) {
		// sleep(1);
		rec = get_line_segment(csd);
		cout << "" << rec;
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

void	create_socket() {
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	struct sockaddr_in	sin = { };
	struct sockaddr_in	sout = { };
	socklen_t			sout_len;
	int					sd;
	int					csd;
	sin.sin_family = PF_INET;
	sin.sin_port = htons(4445);
	sin.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << sin.sin_addr.s_addr << endl;

	std::signal(SIGINT, socket_dispose);
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
	receive_data(csd);
	// if (errno)
	// 	perror("accept()");
	// errno = 0;
	socket_dispose(0);
}

int main(int argc, char const *argv[]) try {
	// if (argc != 3)
	// 	throw (runtime_error(E_ARG));
	create_socket();
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
	return -1;
}
