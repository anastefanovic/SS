#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <vector>
#include <string>
#include <exception>
#include <map>

#include "operand.h"
#include "relocation.h"
#include "symtable.h"
#include "token.h"



static inline bool isSymboInUndefSection(const Symbol& sym)
{
	return sym.getSection() == UNDEF_SECTION;
}

class AssemblerException : std::exception
{
public:
	AssemblerException(std::string message)
		: m_message(message)
	{

	}
	const char* what() const noexcept override
	{
		return m_message.c_str();
	}
private:
	std::string m_message;
};


class Assembler
{
public:
	Assembler();


	void defineLabel(std::string label_name, bool is_defined = true);
	
	void defineGlobalExtern(std::string label_name);

	void defineSection(std::string section_name);

	// za simbole
	void defineData(int size, std::string label_name);
	// za literale
	void defineData(int size, int value);
	
	void defineSkip(int size);

	void equDefinition(std::string symbol_name, const std::vector<Token>& infix_expr);


	void instructionDefinition(const Token& instruction, const Operand& op1 = Operand(), const Operand& op2 = Operand());

	Symbol& getSymbol(std::string symbol_name);


	void resolveForwardReferences();

	void print(FILE *handle = stdout);

	void endDefinition();
private:
	int lc;
	std::string current_section = UNDEF_SECTION;
	std::vector<Symbol> symbol_table;

	void writeData(std::string section_name, int value, int data_size);
	void writeData(std::string section_name, int position, int value, int data_size);
	int  readData(std::string section_name, int position, int data_size);
	void writeRelocation(std::string section_name, int lc, Symbol& symbol, RelocationType reloc_type = RelocationType::ABSOLUTE);

	void processInstructionOperand(const Operand& op, int op_size);

	void processRegisterAddressing(const Operand& op, int op_size);
	void processImmediateAddressing(const Operand& op, int op_size);

	bool isPcReg(const Token& reg);

	std::map<std::string, std::vector<Relocation>> relocation_table;
	std::map<std::string, std::vector<unsigned char>> data_table;


};

#endif // !_ASSEMBLER_H_
