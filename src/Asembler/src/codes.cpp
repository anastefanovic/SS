#include "codes.h"

unsigned char Codes::getRegisterCode(const Token& register_token)
{
	if (register_token.getTokenType() != TokenType::REGISTER)
	{
		return 0;
	}

	return getRegisterCode(register_token.getValue());
}

unsigned char Codes::getRegisterCode(const std::string& register_name)
{
	// uzmemo poslednji karakter da vidimo je li h ili l
	char last_char = register_name[register_name.length() - 1];
	// ukoliko je %pc to je ekvivalent registru %r7
	unsigned char register_num = (register_name == "%pc") ? 7 : (register_name[2] - '0');

	// na drugom mestu se nalazi broj registra
	// format je : %r<num>(h|l) | %pc
	return ((register_num) << 1) | (last_char == 'h');
}

unsigned char Codes::getAddressingCode(AddressingType addressingType)
{
	if (addressingType == AddressingType::RDIRECT)
		return 0x01;
	else if (addressingType == AddressingType::RINDIRECT)
		return 0x02;
	else if (addressingType == AddressingType::IMMEDIATE)
		return 0x00;
	else if (addressingType == AddressingType::MEMDIR)
		return 0x04;
	else // registarsko indirektno sa pomerajem
		return 0x03;
}

unsigned char Codes::formatOperandDescriptor(const Operand& op)
{
	unsigned char addressing_code = getAddressingCode(op.getAddressingType()) << 5;
	unsigned char register_code = (op.getAddressingType() != AddressingType::REGINDIRECT_DISPL) ? getRegisterCode(op.getFirstParam())
																								: getRegisterCode(op.getSecondParam());

	return addressing_code | register_code;
}
