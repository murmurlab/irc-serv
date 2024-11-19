#if !defined(INSTRUCTION_HPP)
# define INSTRUCTION_HPP

#include "Message.hpp"
# include <cstring>
# include <string>
# include <vector>

using std::string;
using std::vector;

typedef enum e_operation
{
	REGISTER,
	EMIT,
	AUTH,
	JOIN,
	SEND,
	VOID
}			e_operation;

class Instruction {
public:
	e_operation	opr;

	string		receiver;
	
	Message		msg;
				Instruction(); // Default constructor, void instructions
};

#endif // INSTRUCTION_HPP
