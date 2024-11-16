#include "GetLineSegment.hpp"

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
		_ab[1] = std::strstr(_ab[0], "\r\n");
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
	_seg = string(_ab[0], (_ab[1] - _ab[0]) + 2);
	_ab[0] = _ab[1] + 2;
}
