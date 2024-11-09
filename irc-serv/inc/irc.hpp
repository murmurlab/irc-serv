#if !defined(IRC_HPP)
# define IRC_HPP

# include "common.hpp"
# include "errors.hpp"
// # include <algorithm>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include <unistd.h>
# include <cstring>
# include <csignal>
# include <cstdlib>
# include <sstream>

# include <pthread.h>

# define IPv4(a, b, c, d) (a << 0 | b << 8 | c << 16 | d << 24)

# define BUF_LEN	512
# define PORT_NUM	4445

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

typedef struct	s_irc {
	// char			*(s_q_matcher[Q_END]);
	// char			*(s_a_matcher[Q_END]);
	unsigned short	port;
	string			pass;
	
}				t_irc;

#endif // IRC_HPP
