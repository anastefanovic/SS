#ifndef _CODES_H_
#define _CODES_H_

#include <string>

#include "token.h"
#include "operand.h"


class Codes
{
public:
	static unsigned char getRegisterCode(const Token& register_token);
	static unsigned char getRegisterCode(const std::string& register_name);
	static unsigned char getAddressingCode(AddressingType addressingType);

	static unsigned char formatOperandDescriptor(const Operand& op);
};
#endif // !_CODES_H_
