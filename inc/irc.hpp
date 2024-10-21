#if !defined(IRC_HPP)
# define IRC_HPP

# include "common.hpp"

# include <sys/socket.h>

typedef struct	s_irc {
	unsigned short	port;
	string			pass;
	
}				t_irc;

#endif // IRC_HPP
