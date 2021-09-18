#pragma once


#ifndef _INSTRUCIONS_H_
#define _INSTRUCTIONS_H_

#include <map>
#include <vector>

#include "operand.h"
#include "token.h"


struct InstructionDefinition
{
	int op_num;

	bool isJmp;

	int op_code;
};

class InstructionsUtil
{
public:
	static bool isValid(const Token& instr_token, const Operand& op1 = Operand(), const Operand& op2 = Operand());
	static bool instructionExists(const Token& instr_token);

	static char getInstructionSuffix(const Token& instr_token);

	static std::string getRootInstructionName(const Token& instr_token);

	static bool isRegisterAddressing(const Operand& operand);

	//static bool isMemIndirect(const Operand& operand);

	static char formatInstructionByte(const Token& instr_token);

	static bool isJmpInstruction(const Token& instr_token);

private:
	static std::map<std::string, InstructionDefinition> instruction_definitions;
};


#endif
