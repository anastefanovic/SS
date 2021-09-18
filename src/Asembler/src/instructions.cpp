#include <algorithm>

#include "instructions.h"

static std::vector<AddressingType> jmp_forbidden = { AddressingType::IMMEDIATE };
static std::vector<AddressingType> regular_forbidden = {  };

std::map<std::string, InstructionDefinition> InstructionsUtil::instruction_definitions =
{
	{"halt", {0, false, 0}},
	{"iret", {0, false, 1}},
	{"ret",  {0, false, 2}},
	{"int",  {1, true, 3}},
	{"call", {1, true, 4}},
	{"jmp",  {1, true, 5}},
	{"jeq",  {1, true, 6}},
	{"jne",  {1, true, 7}},
	{"jgt",  {1, true, 8}},
	{"push",  {1, false, 9}},
	{"pop", {1, false, 10}},
	{"xchg", {2, false, 11}},
	{"mov", {2, false, 12}},
	{"add", {2, false, 13}},
	{"sub", {2, false, 14}},
	{"mul", {2, false, 15}},
	{"div", {2, false, 16}},
	{"cmp", {2, false, 17}},
	{"not", {2, false, 18}},
	{"and", {2, false, 19}},
	{"or", {2, false, 20}},
	{"xor", {2, false, 21}},
	{"test", {2, false, 22}},
	{"shl", {2, false, 23}},
	{"shr", {2, false, 24}}
};

bool InstructionsUtil::isValid(const Token& instr_token, const Operand& op1, const Operand& op2)
{
	if (!instructionExists(instr_token))
		return false;

	int num_operands = (op1.getAddressingType() != AddressingType::NONE) + (op2.getAddressingType() != AddressingType::NONE);

	const auto& instr_def = instruction_definitions[getRootInstructionName(instr_token)];

	if (num_operands != instr_def.op_num)
	{
		return false;
	}

	// Nemamo sta dalje da gledamo, vracamo false.
	if (num_operands == 0)
		return true;

	// ukoliko je jmp instrukcija, uzimamo niz zabranjenih za jmp, u suprotnom za ostale instrukcije
	const auto& forbidden_instrs = (instr_def.isJmp) ? jmp_forbidden : regular_forbidden;

	// Ukoliko je nasao adresiranje u nizu, to znaci da to adresiranje ne sme za skokove, prijavimo gresku.
	if (std::find(forbidden_instrs.begin(), forbidden_instrs.end(), op1.getAddressingType()) !=  forbidden_instrs.end())
		return false;

	// zavrsili smo ukoliko je jmp, jer ima samo jednog operanda.
	if (instr_def.isJmp)
		return true;
	
	
	// provera drugog operanda - Znamo da su obicne instrukcije ( nisu instrukcije sa skokom)
	if (std::find(regular_forbidden.begin(), regular_forbidden.end(), op2.getAddressingType()) != regular_forbidden.end())
		return false;

	// SPEC slucajevi
	// xchng i pop imaju destinacione operande, tako da ne moze biti prvi operand sa neposrednom vrednoscu.
	if ((instr_token.getValue() == "pop" || instr_token.getValue() == "xhcng") && op1.getAddressingType() == AddressingType::IMMEDIATE)
		return false;

	// Compare moze sva obicna adresiranja
	if (instr_token.getValue() == "cmp")
		return true;

	// Drugi operand ostalih instrukcija NE SME biti neposredno adresiran
	if (instr_def.op_num == 2 && op2.getAddressingType() == AddressingType::IMMEDIATE)
		return false;

	return true;
}


bool InstructionsUtil::instructionExists(const Token& instr_token)
{
	// TODO: B ili W Sufix mora da se proveri!!!!!!!!!!!!!!!!!!!
	return instruction_definitions.find(getRootInstructionName(instr_token)) != instruction_definitions.end();
}


char InstructionsUtil::getInstructionSuffix(const Token& instr_token)
{
	if (instr_token.getValue() == "sub")
	{
		return '0';
	}

	char c = instr_token.getValue()[instr_token.getValue().size() - 1];
	
	if (c == 'b')
		return 'b';
	else if (c == 'w')
		return 'w';
	else
		return '0';
}


std::string InstructionsUtil::getRootInstructionName(const Token& instr_token)
{
	char c = getInstructionSuffix(instr_token);

	if (c != '0')
		return instr_token.getValue().substr(0, instr_token.getValue().size() - 1);
	else
		return instr_token.getValue();
}

bool InstructionsUtil::isRegisterAddressing(const Operand& operand)
{
	AddressingType type = operand.getAddressingType();
	return type == AddressingType::RDIRECT || type == AddressingType::RINDIRECT;
}

char InstructionsUtil::formatInstructionByte(const Token& instr_token)
{
	char c = getInstructionSuffix(instr_token);

	auto instr_name = getRootInstructionName(instr_token);

	int op_code = instruction_definitions[instr_name].op_code;

	// Format: OC4 OC3 OC2 OC1 OC0 S 0 0
	// gde je OCn => bit operacionog koda, a S oznacava velicinu instrukcije (S = 1 je 2B, S = 0 je 1B)
	return (op_code << 3) | ((c != 'b') << 2);
}

bool InstructionsUtil::isJmpInstruction(const Token& instr_token)
{
	for (auto& idef : instruction_definitions)
	{
		if (idef.first == instr_token.getValue())
			return idef.second.isJmp;
	}

	return false;
}

