#include <fstream>
#include <iostream>

#include "assembler.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

using namespace std;

/// <summary>
/// TODO, ostalo nam da dodamo relokacije u asembler i obradjujemo ih. Pa nastavljamo dalje sa parser akcijama.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cout << "Invalid number of arguments" << endl;
		cout << "Usage of program: -o <out_file> <input_file>" << endl;
	}

	std::ifstream fhandle(argv[3]);
	FILE* fout = fopen(argv[2], "w");

	std::vector<Token> tokens;
	Lexer lexer(fhandle);
	Assembler assembler;
	Parser parser(assembler);
	
	Token t = lexer.lex();
	while (t.getTokenType() != TokenType::END_OF_FILE)
	{
		tokens.push_back(t);

		t = lexer.lex();
	}

	tokens.push_back(t); // dodamo end of file token.

	parser.start(tokens);

	assembler.print(fout);

	return 0;
}