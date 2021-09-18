#ifndef _POSTFIX_EXPR_H_
#define _POSTFIX_EXPR_H_

#include <vector>
#include <string>

#include "assembler.h"

class ExprEval
{
public:
	enum class ResultType {CONSTANT, LABEL};
	struct Result
	{
		int value;
		ResultType result_type;
		const Symbol* relocation_symbol;

		Result(int value, ResultType result_type, const Symbol* relocation_symbol);
	};

	ExprEval(Assembler& assembler);

	Result evaluateExpr(std::vector<Token> infix_expr);


private:
	Assembler& m_assembler;

	Result createResult(const Token& token);
	Result addition(const Result& left, const Result& right);
	Result subtraction(const Result& left, const Result& right);

	Result eval(const Result& left, const Result& right, const Token& op);
};
#endif // !_POSTFIX_EXPR_H_
