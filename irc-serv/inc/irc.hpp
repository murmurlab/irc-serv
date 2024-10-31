#if !defined(IRC_HPP)
# define IRC_HPP

# include "common.hpp"
# include "errors.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include <unistd.h>
# include <string>

# define IPv4(a, b, c, d) (a << 0 | b << 8 | c << 16 | d << 24)

typedef union	u_ipv4 {
	struct in_addr	s_addr;
	char			arr[4];
}		t_ipv4;

typedef struct	s_irc {
	
	unsigned short	port;
	string			pass;
	
}				t_irc;

#endif // IRC_HPP
