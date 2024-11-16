#include "Parser.hpp"
#include <algorithm>

void	Parser::_token_commmon() {
	_sel_parser++;
}

void	Parser::_token_prefix() {
	string	prefix;
	
	_token_commmon();
	if (_ss.rdbuf()->sgetc() != ':')
		return ;
	// _ss >> _msg->prefix;
	_ss >> prefix;
	string::iterator i1 = std::find(prefix.begin(), prefix.end(), '@');
	if (i1 == prefix.end()) {
		_msg->prefix.nick = new string(prefix.begin() + 1, prefix.end());
		return ;
	}
	string::iterator i2 = std::find(prefix.begin(), prefix.end(), '!');
	if (i2 == prefix.end()) {
		_msg->prefix.nick = new string(prefix.begin() + 1, i2);
		_msg->prefix.host = _msg->prefix.nick;
		return ;
	}
	_msg->prefix.nick = new string(prefix.begin() + 1, i2);
	_msg->prefix.user = new string(i2 + 1, i1);
	_msg->prefix.host = new string(i1 + 1, prefix.end());
}

void	Parser::_token_command() {
	// string	command;
	
	_token_commmon();
	_ss >> _msg->command;
}

// :nick!user@host CAP LS 300 :sasl
void	Parser::_token_params() {
	// _token_commmon();
	string	a_param;
	_ss.get();
	if (_ss.rdbuf()->sgetc() == ':') {
		_msg->trailing = new string;
		std::getline(_ss, *_msg->trailing, '\0');
	} else {
		_ss >> a_param;
		_msg->params.push_back(a_param);
	}
}
void	Parser::_token_unknown() {
	// _sel_parser = PARSE_PREFIX;;
	// cout << "unknown" << endl;
	// cout << _ss.rdbuf() << endl;
}

void	Parser::print_msg(Message &msg) {
	std::ostringstream	os;
	
	if (!_msg->prefix.nick)
		goto nextprint;
	os << *_msg->prefix.nick;
	if (_msg->prefix.user)
		os << "!" << *_msg->prefix.user;
	if (_msg->prefix.host)
		os << "@" << *_msg->prefix.host;
	nextprint:
	cout << "PREFIX:	" << os.str() << endl;
	cout << "COMMAND:	" << _msg->command << endl;
	cout << "PARAMS" << endl;
	for(size_t i = 0; i < _msg->params.size(); i++) {
		cout << "		[" << i << "]: " << _msg->params[i] << endl;
	}
	cout << "TRAILING:	" << (_msg->trailing ? *_msg->trailing : "") << endl;
}

void	Parser::parse() {
	_lexer();

}

void Parser::_lexer() {
	while (2) {
		// sleep(1);
		// component.clear();
		// (ss.rdbuf()->sgetc() == ':')
		_gls.seg = _gls.get_line_segment();
		// _gls.seg.erase(_gls.seg.end() - 2, _gls.seg.end());
		// _gls.seg.end() - 1;
		msgs.resize(msgs.size() + 1);
		_msg = &msgs.back();

		cout << _gls._desc << "> " << *_gls.seg << endl;

		_ss.str(*_gls.seg);
		while (!_ss.eof()) {
			// cout << _sel_parser << "======================================WHILE======================================" << endl;
			usleep(100000);
			switch (_sel_parser) {
			case PARSE_PREFIX:
				_token_prefix();
				break ;
			case PARSE_COMMND:
				_token_command();
				break ;
			case PARSE_PARAMS:
				_token_params();
				break ;
			default:
				_token_unknown();
				break ;
			}
		}
		_sel_parser = PARSE_PREFIX;;
		_ss.clear();
		// _msg->print_msg(*_msg);
		print_msg(*_msg);
		// delete msg;
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

Parser::Parser(int desc): _gls(desc), _sel_parser(PARSE_PREFIX) {

}

