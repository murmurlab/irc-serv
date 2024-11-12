#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "irc.hpp"

class Client
{
	private:
		string _name;
		int _socket;
		bool _is_admin;
	public:
		Client();
		~Client();
		Client(string name, int socket);
		string get_name();
		int get_socket();
		bool is_admin();
		void set_name(string name);
		void set_socket(int socket);
		void set_admin(bool is_admin);
};

#endif // CLIENT_HPP
