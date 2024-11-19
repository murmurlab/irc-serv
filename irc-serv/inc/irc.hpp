#if !defined(IRC_HPP)
# define IRC_HPP

// # include "common.hpp"

// # include "errors.hpp"
// # include <algorithm>

// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <netdb.h>
// # include <arpa/inet.h>
// # include <netdb.h>
// # include <cerrno>
# include <unistd.h>
// # include <cstring>
// # include <csignal>
// # include <cstdlib>
// # include <sstream>

# include <stdexcept>
# include <iostream>
# include <pthread.h>

using std::exception;
using std::endl;
using std::string;
using std::runtime_error;
using std::cout;
using std::cerr;

# include "Server.hpp"
# include "Lexer.hpp"

# define IPv4(a, b, c, d) (a << 0 | b << 8 | c << 16 | d << 24)

# define PORT_NUM	4445

# define _IRC_E_ARG	"Usage ABNF: ./ircserv SHORT_INT \" \" *ALPHA"

# define CAP_SASL		"sasl"

# define S_Q_CAP_LS_302	"CAP LS 302"
# define S_A_CAP_LS_302	"CAP * LS :sasl"
# define S_Q_CAP_REQ	"CAP REQ"
typedef union	u_ipv4 {
	struct in_addr	s_addr;
	char			arr[4];
}		t_ipv4;

// typedef enum	e_q_matcher {
// 	Q_START,
// 	Q_CAP_LS_302,
// 	Q_END
// }

typedef struct	s_parser {
	size_t				sel_parser;
	std::stringstream	ss;
	string				line;
}	t_parser;

typedef struct	s_irc {
	// char			*(s_q_matcher[Q_END]);
	// char			*(s_a_matcher[Q_END]);
	unsigned short	port;
	string			pass;
	
}				t_irc;

#endif // IRC_HPP
