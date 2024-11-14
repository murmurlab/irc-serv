// #include "common.hpp"
#include "Server.hpp"
#include "irc.hpp"

int main(int argc, char const *argv[]) try {
	// if (argc != 3)
	// 	throw (runtime_error(E_ARG));
	// t_irc	irc = {
	// 	.s_q_matcher = {
	// 		[Q_CAP_LS_302] = S_Q_CAP_LS_302,

	// 	}
	// };
	Server	s1("10.11.32.10", 4445);
	s1.~Server();
	// create_socket();
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
	return -1;
}
