#include "Lexer.hpp"
#include "Client.hpp"
#include <algorithm>
#include <string>

void	Lexer::_token_commmon() {
	_sel_lexer++;
}

void	Lexer::_token_prefix() {
	string	prefix;
	
	_token_commmon();
	if (_ss.rdbuf()->sgetc() != ':')
		return ;
	// _ss >> _msg->prefix;
	_ss >> prefix;
	string::iterator i1 = std::find(prefix.begin(), prefix.end(), '@');
	if (i1 == prefix.end()) {
		_msg->prefix.nick.assign(prefix.begin() + 1, prefix.end());
		return ;
	}
	string::iterator i2 = std::find(prefix.begin(), prefix.end(), '!');
	if (i2 == prefix.end()) {
		_msg->prefix.nick.assign(prefix.begin() + 1, i2);
		_msg->prefix.host = _msg->prefix.nick;
		return ;
	}
	_msg->prefix.nick.assign(prefix.begin() + 1, i2);
	_msg->prefix.user.assign(i2 + 1, i1);
	_msg->prefix.host.assign(i1 + 1, prefix.end());
}

void	Lexer::_token_command() {
	_token_commmon();
	_ss >> _msg->command;
}

// :nick!user@host CAP LS 300 :sasl
void	Lexer::_token_params() {
	// _token_commmon();
	string	a_param;
	_ss.get();
	if (_ss.rdbuf()->sgetc() == ':') {
		_ss.get();
		std::getline(_ss, _msg->trailing, '\0');
	} else {
		_ss >> a_param;
		_msg->params.push_back(a_param);
	}
}
void	Lexer::_token_unknown() {
	// _sel_parser = PARSE_PREFIX;;
	// cout << "unknown" << endl;
	// cout << _ss.rdbuf() << endl;
}

void	Lexer::print_msg(Message &msg) {
	std::ostringstream	os;
	
	if (msg.prefix.nick.empty())
		goto nextprint;
	os << msg.prefix.nick;
	if (!msg.prefix.user.empty())
		os << "!" << msg.prefix.user;
	if (!msg.prefix.host.empty())
		os << "@" << msg.prefix.host;
	nextprint:
	cout << "	PREFIX:	" << os.str() << endl;
	cout << "	COMMAND:	" << msg.command << endl;
	cout << "	PARAMS" << endl;
	for(size_t i = 0; i < msg.params.size(); i++) {
		cout << "		[" << i << "]: " << msg.params[i] << endl;
	}
	cout << "	TRAILING:	" << (!msg.trailing.empty() ? msg.trailing : "") << endl;
}

void	Lexer::lex() {
	_tokenize();
}

void Lexer::_tokenize()
{
    while (2) {
		// sleep(1);
		// component.clear();
		// (ss.rdbuf()->sgetc() == ':')
		_gls.seg = _gls.get_line_segment(); // delete while, function should parse only one line
		// _gls.seg.erase(_gls.seg.end() - 2, _gls.seg.end());
		// _gls.seg.end() - 1;
		msgs.resize(msgs.size() + 1);
		_msg = &msgs.back();

		cout << "<" << _gls._desc << " " << *_gls.seg << endl;

		_ss.str(*_gls.seg);
		while (!_ss.eof()) {
			// cout << _sel_parser << "======================================WHILE======================================" << endl;
			usleep(100000);
			switch (_sel_lexer) {
			case LEX_PREFIX:
				_token_prefix();
				break ;
			case LEX_COMMND:
				_token_command();
				break ;
			case LEX_PARAMS:
				_token_params();
				break ;
			default:
				_token_unknown();
				break ;
			}
		}
		_sel_lexer = LEX_PREFIX;;
		_ss.clear();
		delete _gls.seg;
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

Lexer::Lexer(int desc): _sel_lexer(LEX_PREFIX), _gls(desc) {

}

