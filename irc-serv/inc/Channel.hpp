#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "irc.hpp"
#include "Client.hpp"
#include <vector>

class Channel
{
	private:
		string _name;
		Client _admin;
		string _mode;
		std::vector<Client> users;
		std::vector<Client> admins;
	public:
		Channel(string name, Client admin);
		~Channel();
		void add_user(Client user);
		void remove_user(Client user);
		void add_admin(Client admin);
		void remove_admin(Client admin);;
		void change_user_to_admin(Client user);
		bool contain_char(const std::string& str, char c);
};

#endif // CHANNEL_HPP

// For example, if the channel is invite-only, (+i), then you may only join if invited

// As part of the protocol, a user may be a part of several channels at once, but a limit of ten (10)
// channels is recommended as being ample for both experienced and novice users.  See section 8.13 for more information on this

