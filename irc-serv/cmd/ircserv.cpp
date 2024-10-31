#include "common.hpp"
#include "irc.hpp"

int create_socket() {
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	struct sockaddr_in	sin = { };
	struct sockaddr_in	sout = { };
	socklen_t			sout_len;
	int					sd;
	sin.sin_family = PF_INET;
	sin.sin_port = 4444;
	sin.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << sin.sin_addr.s_addr << endl;
	
	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		return perror("socket()"), -1;
	// cout << sd << endl;
	if (bind(sd, (sockaddr *)&sin, sizeof(sin)))
		return perror("bind()"), -1;
	cout << "bind: " << inet_ntoa(sin.sin_addr) << endl;
	if (listen(sd, SOMAXCONN) == -1)
		return perror("listen()"), -1;
	cout << "listen: " << sd << endl;
	while (accept(sd, (sockaddr *)&sout, &sout_len), 1) {
		cout << "an accept" << endl;
		readline
		if (errno)
			perror("accept()");
		errno = 0;
	}
	return 0;
}

int main(int argc, char const *argv[]) try {
	// if (argc != 3)
	// 	throw (runtime_error(E_ARG));
	create_socket();
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
}
