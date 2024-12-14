#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ChMember.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include <string>

using std::string;

typedef enum e_mode_ch {
	
}	e_mode_ch;

class Channel {
public:
	vector<class ChMember>	members;
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
	std::size_t			limit;
	struct s_flags
	{
		int i: 1; //invite only
		int t: 1; //topic protect
		int l: 1; //limitted
	}	flags;
	
						Channel(ChMember &client, string &name);
};

#endif // !CHANNEL_HPP
