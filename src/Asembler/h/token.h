#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>


enum class TokenType 
{ 
    GLOBAL, EXTERN, SECTION, DATA_DEF, EQU, DDOT, COMMA, PLUS, 
    MINUS, DOLLAR, MUL, PERCENT, LPAREN, RPAREN, REGISTER, SYMBOL, 
    END, END_OF_FILE, END_OF_LINE, SKIP, LITERAL
};

class Token
{
public:
    Token() {}
    Token(TokenType type, std::string value) :type(type), value(value) {}
    TokenType getTokenType() const { return type; }
    std::string getValue() const { return value; }
private:
    TokenType type;
    std::string value;

};


#endif