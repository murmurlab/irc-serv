#include "irc.hpp"
#include "Parser.hpp"

// todo: to avoid passing a variadic-size value to read-size argument in read(),
// give constant-size argument to read-size argument, In this case it will read
// constant-size byte and will store in the buff1 and if nl is not exist when
// checked nl in buff1 then part of buff1 that not contains nl is stored in
// buff2 then read again and store to buff1. Finally, concatenate buff2 with up
// to the nl part of buff1. buff2 must be read-size bytes in size.
string get_line_segment(int des) {
	static char		buff[BUF_LEN] = { };
	static size_t	req_len = BUF_LEN - 1;
	static size_t	res_len = read(des, buff, req_len);
	static char		*(ab[2]) = {buff, 0};
	string			seg;

	for (; true;) {
		if (res_len == -1)
			throw runtime_error("read(): -1");
		ab[1] = std::strchr(ab[0], '\n');
		// cout << (void *)ab[1] << endl;
		if (ab[1])
			break ;
		if (ab[0] == buff)
			throw runtime_error("non-nl");
		if (res_len < req_len) {
			bzero(buff, BUF_LEN);
			req_len = BUF_LEN - 1;
			res_len = read(des, buff, req_len);
		} else {
			// cout << "before1: " << ab[0] << endl;
			// cout << "before1: " << buff << endl;
			strlcpy(buff, ab[0], (BUF_LEN - 1) - (ab[0] - buff) + 1);
			// cout << "before2: " << buff << endl;
			bzero(buff + ((BUF_LEN - 1) - (ab[0] - buff)), (ab[0] - buff));
			// cout << "before3: " << buff << endl;
			req_len = BUF_LEN - ((BUF_LEN - 1) - (ab[0] - buff)) - 1;
			res_len = read(des, buff + ((BUF_LEN - 1) - (ab[0] - buff)), req_len);
			// cout << "readed: " << buff << endl;
		}
		ab[0] = buff;
		continue ;

	}
	seg = string(ab[0], (ab[1] - ab[0]) + 1);
	ab[0] = ab[1] + 1;
	return seg;
}

void	parse_commmon(int &sel_parser) {
	sel_parser++;
}

void	parse_prefix(std::stringstream &ss) {
	string				prefix;

	if (ss.rdbuf()->sgetc() != ':')
		return ;
	ss >> prefix;
	cout << "prefix" << endl;
	// parse_commmon(sel_parser);
}
void	parse_command(std::stringstream &ss) {
	string				command;

	ss >> command;
	cout << "command " << command << endl;
	// parse_commmon(sel_parser);
}
void	parse_params(std::stringstream &ss) {
	string				trailing;
	string				param;

	if (ss.rdbuf()->sgetc() == ':')
		std::getline(ss, trailing);
	else
		ss >> param;
	cout << "param / trailing " << param << "/" << trailing << endl;
}
void	parse_unknown(std::stringstream &ss) {
	cout << "unknown" << endl;
}

Parser::Parser(int csd): _csd(csd) {
	_parse();
}

void Parser::_parse() {
	// parser.sel_parser = 0;
	while (2) {
		// sleep(1);
		// component.clear();
		// ss.clear();
		// (ss.rdbuf()->sgetc() == ':')
		_line = get_line_segment(_csd);
		cout << "> " << _line;

		_ss.str(_line);
		cout << "str: " << _ss.str();
		// std::getline(ss, component, ' ');
		// ss >> component;
		// cout << "component: " << component << endl;
		// cout << "rdbuf: " << ss.rdbuf()->str();
		// cout << "rdbuf: " << ss.rdbuf()->str();
		// cout << "rdbuf: " << ss.rdbuf();
		// cout << "rdbuf all: " << ss.rdbuf();
		// std::getline(ss, trailing);
		// cout << "trailing; " << trailing << endl;
		// cout << "rdbuf: " << ss.rdbuf(); 
		// sel_parser = (component.at(0) == ':') ? 0 : 1;
		continue ;
		// return ;
		while (!_ss.eof()) {
			cout << "======================================WHILE======================================" << endl;
			switch (_sel_parser) {
			case 0:
				parse_prefix(_ss);
				break ;
			case 1:
				parse_command(_ss);
				break ;
			case 2:
				parse_params(_ss);
				break ;
			default:
				parse_unknown(_ss);
				return ;
				break ;
			}
		}
		// if (rec == S_Q_CAP_LS)
		// 	write(*(int *)csd, S_A_CAP_LS, sizeof(S_A_CAP_LS));
		// getchar();
	}
	
	// size_t				size;
	// // sleep(7);
	// for (char buff[BUF_LEN] = {}; true; ) {
	// 	lseek(csd, 0, SEEK_SET);
	// 	if ((size = read(csd, buff, sizeof(buff) - 1)) == -1)
	// 		throw runtime_error("read(): " + string(strerror(errno)));
	// 	cout << buff;
	// 	// cout << "byte: " << size << " data: " << buff << endl;
	// 	bzero(buff, BUF_LEN);
	// }
	
}
