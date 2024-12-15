#include "GetLineSegment.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unistd.h>

GetLineSegment::~GetLineSegment() {
	// delete seg;
	close(_desc);
}

GetLineSegment::GetLineSegment(int desc_): _d_sub("\r\n"), _desc(desc_) {
	seg = NULL;
	// _ab[0] = _buff;
	// _ab[1] = 0;
}


string *GetLineSegment::get_line_segment() {
	string	*tmp;
	char	buff[REQ_LEN + 1];

	while (1) {
		std::memset(buff, 0, REQ_LEN + 1);
		_res_len = read(_desc, buff, REQ_LEN);
		if ((_res_len == (size_t)-1) && (errno == EWOULDBLOCK))
			throw ewouldblock("read(): -1");
		if (_res_len == 0) {
			std::cerr << "read(): 0, fd: " << _desc << endl;
			throw std::runtime_error("read(): 0"); // client hang up
		}
		_left = (_left + buff);
		_d_pos = _left.find(_d_sub);
		if (_d_pos == string::npos)
			continue ;
		if (std::distance(_left.begin(), _left.begin() + _d_pos) > MAX_LEN)
			throw std::runtime_error("exceed element throwu");
		tmp = new string(_left.begin(), _left.begin() + _d_pos);
		_left.erase(_left.begin(), _left.begin() + (_d_pos + _d_sub.length()));
		return tmp;
		// std::cout << _left << std::endl;
		
	}
}

