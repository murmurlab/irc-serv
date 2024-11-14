#include "irc.hpp"
#include <cstring>
#include "Parser.hpp"

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

// Parser::Parser(int csd): _csd(csd) {
// 	_parse();
// }

void Parser::_parse() {
	// parser.sel_parser = 0;
	while (2) {
		// sleep(1);
		// component.clear();
		// ss.clear();
		// (ss.rdbuf()->sgetc() == ':')
		_gls.get_line_segment();
		cout << _gls._desc << "> " << _gls._seg;

		_ss.str(_gls._seg);
		// cout << "str: " << _ss.str();

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
			case PARSE_PREFIX:
				parse_prefix(_ss);
				break ;
			case PARSE_COMMND:
				parse_command(_ss);
				break ;
			case PARSE_PARAMS:
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

Parser::Parser(int desc): _gls(desc) {

}

GetLineSegment::GetLineSegment(int desc_): _desc(desc_) {
	std::memset(_buff, 0, sizeof(_buff));
	_req_len = BUF_LEN - 1;
	_res_len = -1;
	_ab[0] = _buff;
	_ab[1] = 0;
}

// todo: to avoid passing a variadic-size value to read-size argument in read(),
// give constant-size argument to read-size argument, In this case it will read
// constant-size byte and will store in the buff1 and if nl is not exist when
// checked nl in buff1 then part of buff1 that not contains nl is stored in
// buff2 then read again and store to buff1. Finally, concatenate buff2 with up
// to the nl part of buff1. buff2 must be read-size bytes in size.
void GetLineSegment::get_line_segment() {
	for (; true;) {
		if (_res_len == -1) {
			_res_len = read(_desc, _buff, _req_len);
			if (_res_len == -1)
				throw IRC_MsgIncomplate("read(): -1");
		}
		if (_res_len == 0)
			cout << "read(): 0" << endl;
		_ab[1] = std::strchr(_ab[0], '\n');
		// cout << (void *)ab[1] << endl;
		if (_ab[1])
			break ;
		if (_ab[0] == _buff)
			throw runtime_error("non-nl");
		if (_res_len < _req_len) {
			bzero(_buff, BUF_LEN);
			_req_len = BUF_LEN - 1;
			_res_len = read(_desc, _buff, _req_len);
		} else {
			// cout << "before1: " << ab[0] << endl;
			// cout << "before1: " << buff << endl;
			strlcpy(_buff, _ab[0], (BUF_LEN - 1) - (_ab[0] - _buff) + 1);
			// cout << "before2: " << buff << endl;
			bzero(_buff + ((BUF_LEN - 1) - (_ab[0] - _buff)), (_ab[0] - _buff));
			// cout << "before3: " << buff << endl;
			_req_len = BUF_LEN - ((BUF_LEN - 1) - (_ab[0] - _buff)) - 1;
			_res_len = read(_desc, _buff + ((BUF_LEN - 1) - (_ab[0] - _buff)), _req_len);
			// if (res_len == -1)
			// 	cerr << "read(): -1"
			// cout << "readed: " << buff << endl;
		}
		_ab[0] = _buff;
		continue ;

	}
	_seg = string(_ab[0], (_ab[1] - _ab[0]) + 1);
	_ab[0] = _ab[1] + 1;
}
