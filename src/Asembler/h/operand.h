#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "token.h"

enum class AddressingType
{
	IMMEDIATE, RDIRECT, RINDIRECT, REGINDIRECT_DISPL, MEMDIR,
	NONE
};

class Operand
{
private:
	AddressingType m_addressing_type = AddressingType::NONE;

	Token m_first_param;
	Token m_second_param;

public:
	Operand(AddressingType addressing_type, Token first, Token second = Token()): 
		m_addressing_type(addressing_type), m_first_param(first), m_second_param(second) {}
	Operand() {}

	const Token& getFirstParam() const { return m_first_param; }
	const Token& getSecondParam() const { return m_second_param; }

	void setFirstParam(const Token& token) { m_first_param = token; }
	void setSecondParam(const Token& token) { m_second_param = token; }

	AddressingType getAddressingType() const { return m_addressing_type; }
};







#endif
