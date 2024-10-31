#include "common.hpp"
#include "irc.hpp"

int create_socket() {
	const static size_t P_IP =	getprotobyname("ip")->p_proto; // use tcp
	struct sockaddr_in	sock = { };
	socklen_t			sout_len;
	int					sd;
	sock.sin_family = PF_INET;
	sock.sin_port = 4444;
	sock.sin_addr.s_addr = *(int *)"\x7f\x00\x00\x01";
	// IPv4(127, 0, 0, 1)
	// cout << inet_addr("127.0.0.1") << endl;
	// cout << sock.sin_addr.s_addr << endl;


	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		return perror("socket()"), -1;
	// if (bind(sd, (sockaddr *)&sock, sizeof(sock)))
	// 	return perror("bind()"), -1;
	if (connect(sd, (sockaddr *)&sock, sizeof(sock)) == -1)
		return perror("connect()"), -1;
	cout << "connect succesfully" << endl;
	// pause();
	for (string line; std::getline(std::cin, line);) {
		dprintf(sd, "%s\n", line.c_str());
	}
	// if (listen(sd, SOMAXCONN) == -1)
	// 	return perror("listen()"), -1;
	// while (accept(sd, (sockaddr *)&sout, &sout_len) == 0);
	return 0;
}


int main(int argc, char const *argv[])
{
	create_socket();
	return 0;
}
