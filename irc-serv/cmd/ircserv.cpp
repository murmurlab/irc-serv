#include "common.hpp"
#include "irc.hpp"

void	reveive_data() {

}

void	create_socket() {
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	struct sockaddr_in	sin = { };
	struct sockaddr_in	sout = { };
	socklen_t			sout_len;
	int					sd;
	int					csd;
	sin.sin_family = PF_INET;
	sin.sin_port = 4445;
	sin.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << sin.sin_addr.s_addr << endl;
	
	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		throw runtime_error("socket()" + string(strerror(errno)));
	// cout << sd << endl;
	if (bind(sd, (sockaddr *)&sin, sizeof(sin)))
		throw runtime_error("bind()" + string(strerror(errno)));
	cout << "bind: " << inet_ntoa(sin.sin_addr) << endl;
	if (listen(sd, SOMAXCONN) == -1)
		throw runtime_error("listen()" + string(strerror(errno)));
	cout << "listen: " << sd << endl;
	if ((csd = accept(sd, (sockaddr *)&sout, &sout_len)) == -1)
		throw runtime_error("accept()" + string(strerror(errno)));
	cout << "an accept" << endl <<
		"\tip: " << inet_ntoa(sout.sin_addr) << endl;
	for (char buff[4] = {}; true; read(csd, buff, 4)) {
		cout << "data: " << buff << endl;
		bzero(buff, 4);
		break ;
	}
	if (errno)
		perror("accept()");
	errno = 0;
}

int main(int argc, char const *argv[]) try {
	// if (argc != 3)
	// 	throw (runtime_error(E_ARG));
	try {
		create_socket();
	} catch (runtime_error &e) {
		cerr << e.what() << endl;
		return -1;
	}
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
}
