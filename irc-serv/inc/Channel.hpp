#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"
#include "Message.hpp"
#include <string>

using std::string;

typedef enum e_mode_ch {
	
}	e_mode_ch;

class Channel {
public:
	vector<class Client *>	clients;
	// vector<Client *>	ops;
/* Channels names are strings (beginning with a '&', '#', '+' or '!'
   character) of length up to fifty (50) characters.  Channel names are
   case insensitive. */
/* SHALL NOT contain any spaces
   (' '), a control G (^G or ASCII 7), a comma (',' which is used as a
   list item separator by the protocol). */
	string				name;
	string				topic;
	string				pass;
	bool				invite_only;
	bool				topic_protect;
	bool				limitted;
	std::size_t			limit;
	
						Channel(Client &client, string &name);
};

#endif // !CHANNEL_HPP
