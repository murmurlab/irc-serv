#if !defined(GET_LINE_SEGMENT_HPP)
# define GET_LINE_SEGMENT_HPP

# include "ExceptionMsgIncomplate.hpp"

# include <sstream>
# include <string>
# include <cstddef>
# include <cstring>
# include <unistd.h>
# include <iostream>
# include <stdexcept>

using std::runtime_error;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

# define MAX_LEN	511
# define REQ_LEN	5

# if (REQ_LEN > MAX_LEN)
# warning "REQ_LEN greater then MAX_LEN"
# endif

class	GetLineSegment {
	typedef IRC_MsgIncomplate 	ewouldblock;
	string						_left;
	size_t						_res_len;
	string						_d_sub;
	string::size_type			_d_pos;
public:
	string						*seg;
	int							_desc;
	string						*get_line_segment();
								GetLineSegment(int desc_);
								~GetLineSegment();
};

#endif // GET_LINE_SEGMENT_HPP
