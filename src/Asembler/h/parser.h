#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>

#include "assembler.h"
#include "operand.h"
#include "token.h"

// ============================ GRAMATIKA =====================================
// Start ::= {[DefinicijaLabele] [Direktiva/Naredba] NEWLINE} END_OF_FILE;
//
// DefinicijaLabele  ::= SIMBOL DVOTACKA;
//
// Direktiva/Naredba ::= Direktiva | Naredba;
//
// Direktiva ::= (GLOBAL | EXTERN | SECTION) SIMBOL{,SIMBOL}
//             | (BYTE | WORD) (SIMBOL | LITERAL){,(SIMBOL | LITERAL)}
//             | SKIP LITERAL
//             | EQU SIMBOL , Izraz
//             ;
//
// Naredba ::= SIMBOL 
//           | (SIMBOL Operand) 
//           | (SIMBOL Operand,Operand)
//
// Operand ::= SIMBOL 
//           | LITERAL 
//           | SIMBOL \( % REGISTAR \) 
//           | LITERAL \( % REGISTAR \) 
//           | \( % REGISTAR \) 
//           | % REGISTAR
//           | * % REGISTAR
//           | * \( % REGISTAR \)
//           | * LITERAL \( % REGISTAR \)
//           | * SIMBOL \( % REGISTAR \)
//           | * LITERAL
//           | * SIMBOL
//           | $ SIMBOL | $ LITERAL 
//
// Izraz ::= [-](SIMBOL | LITERAL) {(+|-) (SIMBOL | LITERAL)};
// ============================================================================
class Parser
{
public:
	Parser(Assembler& assembler);

	void start(const std::vector<Token>& tokens);
private:
	bool expr(const std::vector<Token>& tokens, int& i);
	bool labelDefinition(const std::vector<Token>& tokens, int& i);
	bool addRegister(const std::vector<Token>& tokens, int& i);
	bool commonOperand(const std::vector<Token>& tokens, int& i);
	bool operand(const std::vector<Token>& tokens, int& i);
	bool globalExternDefinition(const std::vector<Token>& tokens, int& i);
	bool sectionDefinition(const std::vector<Token>& tokens, int& i);
	bool dataDefinition(const std::vector<Token>& tokens, int& i);
	bool skipDefinition(const std::vector<Token>& tokens, int& i);
	bool equDefinition(const std::vector<Token>& tokens, int& i);
	bool directiveDefinition(const std::vector<Token>& tokens, int& i);
	bool instructionDefinition(const std::vector<Token>& tokens, int& i);
	
	Assembler& m_assembler;

	Operand m_operands[2];

	int curr_operand = 0;

	std::vector<Token> m_infix_expr;

	// Koristi se kada treba da proverimo je li validan operand za jump
	bool m_validInJmp = false;

	bool m_isMul = false;
};











#endif