#include <iostream>

#include "instructions.h"
#include "operand.h"
#include "parser.h"
#include "utils.h"

using namespace std;

bool Parser::expr(const std::vector<Token>& tokens, int& i)
{
	int local_i = i;
	bool isMinusFirst = tokens[i].getTokenType() == TokenType::MINUS;
	
	if (isMinusFirst)
	{
		local_i++;
	}

	if (tokens[local_i].getTokenType() != TokenType::SYMBOL && tokens[local_i].getTokenType() != TokenType::LITERAL)
	{
		return false;
	}

	std::vector<Token> infix_expr;

	// OBRADI PRVI SLUCAJ
	if (isMinusFirst)
	{
		infix_expr.push_back(Token(TokenType::LITERAL, "0"));
		infix_expr.push_back(tokens[i]);
	}
	infix_expr.push_back(tokens[local_i]);

	local_i++;
	while (tokens[local_i].getTokenType() != TokenType::END_OF_LINE)
	{
		bool isPlusMinus = tokens[local_i].getTokenType() == TokenType::MINUS || tokens[local_i].getTokenType() == TokenType::PLUS;
		bool isLiteralSymbol = tokens[local_i + 1].getTokenType() == TokenType::LITERAL || tokens[local_i + 1].getTokenType() == TokenType::SYMBOL;

		if (!isPlusMinus || !isLiteralSymbol)
			return false;

		// Dodamo sada ostale operande i operatore u infix izraz
		infix_expr.push_back(tokens[local_i]);
		infix_expr.push_back(tokens[local_i + 1]);

		local_i += 2;
	}
	i = local_i;

	m_infix_expr = infix_expr;

	return true;
}

bool Parser::labelDefinition(const std::vector<Token>& tokens, int& i)
{
	// Nije definicija labele
	if (tokens[i].getTokenType() != TokenType::SYMBOL || tokens[i + 1].getTokenType() != TokenType::DDOT)
		return false;

	// Definisemo labelu.
	m_assembler.defineLabel(tokens[i].getValue());

	i = i + 2;

	return true;
}

bool Parser::addRegister(const std::vector<Token>& tokens, int& i)
{

	// Ukoliko nije izmedju zagrada, vracamo false
	if ((tokens[i + 1].getTokenType() != TokenType::LPAREN) ||
		(tokens[i + 2].getTokenType() != TokenType::REGISTER) ||
		(tokens[i + 3].getTokenType() != TokenType::RPAREN))
	{
		return false;
	}

	// preskacemo simbol/literal
	++i;

	// Pretpostavimo u pocetku da je mem indirektno sa pomerajem
	AddressingType addr_type = AddressingType::REGINDIRECT_DISPL;

	// Token ia i je simbol/literal, token ispred i je registar
	m_operands[curr_operand] = Operand(addr_type, tokens[i - 1], tokens[i + 1]);

	// preskacemo 2 simbola.
	i += 3;

	return true;
}

bool Parser::commonOperand(const std::vector<Token>& tokens, int& i)
{
	bool isSuccessful = true;
	switch (tokens[i].getTokenType())
	{
	case TokenType::SYMBOL:
	case TokenType::LITERAL:
	{
		if (!addRegister(tokens, i))
		{
			m_operands[curr_operand] = Operand(AddressingType::MEMDIR, tokens[i]);

			// Moze se koristiti u jmp
			m_validInJmp = true;

			i += 1;
		}
	}
	break;

	case TokenType::REGISTER:
	{
		m_operands[curr_operand] = Operand(AddressingType::RDIRECT, tokens[i]);
		// preskacemo jedan simbol.
		i += 1;
	}
	break;

	case TokenType::LPAREN:
	{
		isSuccessful = tokens[i + 1].getTokenType() == TokenType::REGISTER && tokens[i + 2].getTokenType() == TokenType::RPAREN;

		if (isSuccessful)
		{
			m_operands[curr_operand] = Operand(AddressingType::RINDIRECT, tokens[i + 1]);
			i += 3;
		}
	}
	break;

	default:
	{
		isSuccessful = false;
	}
	}

	return true;
}


bool Parser::operand(const std::vector<Token>& tokens, int& i)
{
	int local_i = i;

	m_isMul = false;
	m_validInJmp = false;

	switch (tokens[i].getTokenType())
	{
	case TokenType::DOLLAR:
	{
		bool isSuccessful = false;
		// Specijalan slucaj.
		if (tokens[i + 1].getTokenType() == TokenType::SYMBOL)
		{
			isSuccessful = true;

		}
		else if (tokens[i + 1].getTokenType() == TokenType::LITERAL)
		{
			isSuccessful = true;
		}

		m_operands[curr_operand] = Operand(AddressingType::IMMEDIATE, tokens[i + 1]);

		i = (isSuccessful) ? i + 2 : i;

		return isSuccessful;
	}

	case TokenType::MUL:
	{
		// naznaciti samo da je * u pitanju, da se zna pri obradi operanda(zbog adresiranja)
		m_validInJmp = true;
		m_isMul = true;
		local_i++;
	}
	break;

	case TokenType::END_OF_LINE:
	{
		m_operands[curr_operand] = Operand();
		return true;
	}
	break;
	}

	bool isSuccessful = commonOperand(tokens, local_i);

	// Azuriraj i, za slucaj da smo uspesno parsirali.
	if (isSuccessful)
		i = local_i;

	return isSuccessful;
}

bool Parser::globalExternDefinition(const std::vector<Token>& tokens, int& i)
{
	int local_i = i + 1;

	if (tokens[local_i].getTokenType() != TokenType::SYMBOL)
		return false;

	// OBRADI SIMBOL;
	m_assembler.defineGlobalExtern(tokens[local_i].getValue());

	local_i++;
	while (tokens[local_i].getTokenType() != TokenType::END_OF_LINE)
	{
		if (tokens[local_i].getTokenType() != TokenType::COMMA || 
			tokens[local_i + 1].getTokenType() != TokenType::SYMBOL)
			return false;

		m_assembler.defineGlobalExtern(tokens[local_i].getValue());
		local_i += 2;
	}
	// sada mozemo da podesimo i na odgovarajucu vrednost.
	i = local_i;

	return true;
}

bool Parser::sectionDefinition(const std::vector<Token>& tokens, int& i)
{
	// Sekcija je definisana kao .section <naziv_sekcije>:\n
	if (tokens[i + 1].getTokenType() != TokenType::SYMBOL || 
		tokens[i + 2].getTokenType() != TokenType::DDOT)
	{
		return false;
	}

	// Definisemo sekciju. Dohvatamo Symbol token, i uzimamo njegov naziv kao vrednost
	// (to je naziv sekcije koju definisemo).
	m_assembler.defineSection(tokens[i + 1].getValue());

	// 3 tokena zauzima definicija sekcije.
	i += 3;

	return true;
}

bool Parser::dataDefinition(const std::vector<Token>& tokens, int& i)
{
	int local_i = i + 1;

	Token data_type_token = tokens[i];
	Token symbol_token = tokens[i + 1];

	// Lambda funkcija, posto imamo ovu obradu na dva mesta.
	auto defineDataFunc = [&](const Token& symbol_token, int data_size) {
		if (symbol_token.getTokenType() == TokenType::LITERAL)
			m_assembler.defineData(data_size, utils::stoi(symbol_token.getValue()));
		else
			m_assembler.defineData(data_size, symbol_token.getValue());
	};

	// PROVERA DA LI JE ISPRAVNA GRAMATIKA
	if (tokens[local_i].getTokenType() != TokenType::SYMBOL && tokens[local_i].getTokenType() != TokenType::LITERAL)
	{
		return false;
	}
	local_i++;

	// Postoje samo .word i .byte. .word je 2B, a .byte je 1B
	int data_size = (data_type_token.getValue() == ".word") ? 2 : 1;

	// OBRADI SLUCAJ - ako je literal value konvertujemo u int
	// u slucaju da je simbol samo prosledimo naziv simbola.
	defineDataFunc(symbol_token, data_size);

	while (tokens[local_i].getTokenType() != TokenType::END_OF_LINE)
	{
		bool isComma = tokens[local_i].getTokenType() == TokenType::COMMA;
		bool isLiteralSymbol = tokens[local_i + 1].getTokenType() == TokenType::LITERAL || tokens[local_i + 1].getTokenType() == TokenType::SYMBOL;
		bool isEndOfLine = tokens[local_i].getTokenType() == TokenType::END_OF_LINE;

		if (!isComma || !isLiteralSymbol)
			return false;

		defineDataFunc(tokens[local_i + 1], data_size);

		local_i += 2;
	}
	i = local_i;

	return true;
}

bool Parser::skipDefinition(const std::vector<Token>& tokens, int& i)
{
	if (tokens[i + 1].getTokenType() != TokenType::LITERAL)
		return false;

	// Obrada slucaja za skip.
	m_assembler.defineSkip(utils::stoi(tokens[i + 1].getValue()));

	// preskacemo .skip i literal uz skip
	i += 2;

	return true;
}

bool Parser::equDefinition(const std::vector<Token>& tokens, int& i)
{
	if (tokens[i + 1].getTokenType() != TokenType::SYMBOL || tokens[i + 2].getTokenType() != TokenType::COMMA)
		return false;

	// Obraditi izraz
	int local_i = i + 3;
	bool isExpr = expr(tokens, local_i);
	// Ukoliko smo uspesno parsirali izraz, definisemo simbol preko .equ direktive
	if (isExpr)
		m_assembler.equDefinition(tokens[i + 1].getValue(), m_infix_expr);

	// Azuriramo indeks tokena.
	i = (isExpr) ? local_i : i;

	return isExpr;
}

bool Parser::directiveDefinition(const std::vector<Token>& tokens, int& i)
{
	bool isSuccessful = false;
	switch (tokens[i].getTokenType())
	{
	case TokenType::EXTERN:
	case TokenType::GLOBAL:
	{
		isSuccessful = globalExternDefinition(tokens, i);
	}
	break;

	case TokenType::SECTION:
	{
		isSuccessful = sectionDefinition(tokens, i);
	}
	break;

	case TokenType::DATA_DEF:
	{
		isSuccessful = dataDefinition(tokens, i);
	}
	break;

	case TokenType::SKIP:
	{
		isSuccessful = skipDefinition(tokens, i);
	}
	break;

	case TokenType::EQU:
	{
		isSuccessful = equDefinition(tokens, i);
	}
	break;
	}

	return isSuccessful;
}

bool Parser::instructionDefinition(const std::vector<Token>& tokens, int& i)
{
	// Sve je OK
	int local_i = i + 1;
	
	curr_operand = 0;
	bool isOperand = operand(tokens, local_i);

	if (!isOperand)
		return false;


	if (m_operands[0].getAddressingType() != AddressingType::NONE)
	{
		if (!m_validInJmp && InstructionsUtil::isJmpInstruction(tokens[i]))
			throw AssemblerException("Not valid addressing for jump instruction");
		else if (m_isMul && !InstructionsUtil::isJmpInstruction(tokens[i]))
			throw AssemblerException("Addressing not valid for non jump instruction");
	}

	// Proveravamo da li nema zarez ispred
	if (tokens[local_i].getTokenType() != TokenType::COMMA)
	{
		// operator sa jednim ili nula operanada
		m_assembler.instructionDefinition(tokens[i], m_operands[0]);
		i = local_i;
		return true;
	}
	local_i++;

	// Slucaj kada su dva operanda

	curr_operand = 1;
	isOperand = operand(tokens, local_i);

	// ovo ce uspesno parsirati i kada nema nista posle, medjutim MORA postojati, jer smo naisli na zarez.
	// stoga dodatno proveravamo da li je m_operands[1] tip adresiranja != None
	if (!isOperand || m_operands[1].getAddressingType() == AddressingType::NONE)
		return false;

	// Proverimo opet da li je validan operand.
	if (m_operands[1].getAddressingType() != AddressingType::NONE)
	{
		if (!m_validInJmp && InstructionsUtil::isJmpInstruction(tokens[i]))
			throw AssemblerException("Not valid addressing for jump instruction");
		else if (m_isMul && !InstructionsUtil::isJmpInstruction(tokens[i]))
			throw AssemblerException("Addressing not valid for non jump instruction");
	}

	// SIGURNO moraju da budu dva operanda.
	m_assembler.instructionDefinition(tokens[i], m_operands[0], m_operands[1]);

	// Azuriramo i;
	i = local_i;

	return true;
}


Parser::Parser(Assembler& assembler)
	: m_assembler(assembler)
{
}

// Start ::= {[DefinicijaLabele] [Direktiva/Naredba] NEWLINE} END_OF_FILE;
void Parser::start(const std::vector<Token>& tokens)
{
	int current_line = 1;
	try {
		int i = 0;
		while (tokens[i].getTokenType() != TokenType::END_OF_FILE)
		{
			labelDefinition(tokens, i);

			if (tokens[i].getTokenType() == TokenType::SYMBOL)
			{
				//cout << "Okay" << endl;
				instructionDefinition(tokens, i);
			}
			else
			{
				directiveDefinition(tokens, i);
				//cout << "Okay" << endl;
			}

			if (tokens[i].getTokenType() != TokenType::END_OF_LINE)
			{
				// TODO Ispisati poruku greske.
				cout << "Neuspesno parsirano" << endl;
				return;
			}
			else
			{
				// cout << "Uspesno parsirano" << endl;

				++current_line;
				i++;
			}
		}

		m_assembler.endDefinition();
		m_assembler.resolveForwardReferences();
	}
	catch (AssemblerException& exc)
	{
		printf("Error at line number %d: %s", current_line, exc.what());
	}

}