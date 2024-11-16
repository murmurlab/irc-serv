#if !defined(GET_LINE_SEGMENT_HPP)
# define GET_LINE_SEGMENT_HPP

# include "ExceptionMsgIncomplate.hpp"

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

# define BUF_LEN	512

class	GetLineSegment {
	char						_buff[BUF_LEN];
	size_t						_req_len;
	size_t						_res_len;
	char						*(_ab[2]);
public:
	int							_desc;
	string						_seg;
	void						get_line_segment();
								GetLineSegment(int desc_);
};

#endif // GET_LINE_SEGMENT_HPP
