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
	EMIT,
	SEND,
	VOID
}			e_operation;

class Client;

class Instruction {
public:
	e_operation	opr;

	vector<Client *>	clients;
	/* 
		additional data fields
	 */
	
	Message		msg;
				Instruction(); // Default constructor, void instructions
};

#endif // INSTRUCTION_HPP
