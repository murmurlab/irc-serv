#include "Channel.hpp"
#include <string.h>

Channel::Channel(string name, Client admin)
{
	_name = name;
	_admin = admin;
	if (contain_char(name, ' ') || contain_char(name, ',') || contain_char(name, 7) || (name.size() > 200 || name.size() < 2))
		throw std::invalid_argument("Channel name must be between 2 and 200 characters, and cannot start with a space.");
	if (name[0] != '#' && name[0] != '&')
		_name = "#" + name;

}

Channel::~Channel()
{

}

bool contain_char(const std::string& str, char ch)
{
    for (char c : str)
	{
        if (c == ch)
            return true;
    }
    return false;
}