#if !defined(MESSAGE_HPP)
# define MESSAGE_HPP

# include <cstring>
# include <string>
# include <vector>

using std::string;
using std::vector;

class Message {
public:
	struct s_prefix {
		string	*nick;
		string	*user;
		string	*host;
	}					prefix;
	string				command;
	std::vector<string>	params;
	string				*trailing;

						Message();
};

#endif // MESSAGE_HPP
