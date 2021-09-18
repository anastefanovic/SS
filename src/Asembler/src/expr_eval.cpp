#include "expr_eval.h"
#include "utils.h"

static bool isConstMinusLabel(const ExprEval::Result& l, const ExprEval::Result& r)
{
	return l.result_type == ExprEval::ResultType::CONSTANT &&
		   r.result_type == ExprEval::ResultType::LABEL;
}

/*
 * Helper metoda za ispitivanje minus operacije, da li su dve sekcije kompatabilne za
 * operaciju minus
 */
static bool areSectionsCompatible(const ExprEval::Result& l, const ExprEval::Result& r)
{
	// Ovaj proverava da su je kod operacije -, dve sekcije ova dva simbola/konstanti kompatabilne
	// kompatabilne su ukoliko je od desnog simbola sekcija CONST_SECTION(secija koja predstavlja literale)
	// ili ukoliko su sekcije levog i desnog simbola iste, ali nijedan od ta dva simbola nije iz predefinisane sekcije
	// za NEDEFINISANE simbole (UNDEF_SECTION)
	return r.relocation_symbol->getSection() == CONST_SECTION ||
		   (l.relocation_symbol->getSection() == r.relocation_symbol->getSection() && l.relocation_symbol->getSection() != UNDEF_SECTION);
}

ExprEval::ExprEval(Assembler& assembler)
	: m_assembler(assembler)
{
}

ExprEval::Result ExprEval::evaluateExpr(std::vector<Token> infix_expr)
{	
	Result result = createResult(infix_expr[0]);
	for (int i = 1; i < infix_expr.size(); i += 2)
	{
		result = eval(result, createResult(infix_expr[i + 1]), infix_expr[i]);
	}

	return result;
}

ExprEval::Result ExprEval::createResult(const Token& token)
{
	if (token.getTokenType() == TokenType::LITERAL)
		return Result(utils::stoi(token.getValue()), ResultType::CONSTANT, &m_assembler.getSymbol(CONST_SECTION));

	const auto& symbol = m_assembler.getSymbol(token.getValue());
	if (symbol.getSymbolType() == SymbolType::EQU)
	{
		// Ukoliko je EQU simbol, njegov relokacioni simbol ce biti CONST_SECTION, ukoliko je 
		// rezultat njegovog izraza BROJ, a u suprotonm ce biti naziv simbola(labela/sekcija)
		// za koji je vezana relokacija.
		return Result(symbol.getValue(),
			          symbol.getRelocationSymbol() == CONST_SECTION ? ResultType::CONSTANT 
			                                                        : ResultType::LABEL,
			          &m_assembler.getSymbol(symbol.getRelocationSymbol()));
	}
	else
	{
		// Ukoliko je simbol tipa labela/sekcija, njegov relokacioni simbol je on sam
		// ukoliko nije definisan ILI sekcija u kojoj je definisan(jer je simbol vec definisan)
		return Result(symbol.getValue(), 
			          ResultType::LABEL, 
			          &symbol);
	}
}

ExprEval::Result ExprEval::addition(const Result& left, const Result& right)
{
	if (left.result_type == ResultType::LABEL && right.result_type== ResultType::LABEL)
	{
		throw AssemblerException("Invalid operand (" + m_assembler.getSymbol(left.relocation_symbol->getName()).getSection() + " and " +
			m_assembler.getSymbol(right.relocation_symbol->getName()).getSection() + ") sections for '+'");
	}

	return Result(left.value + right.value, 
		          (left.result_type != right.result_type) ? ResultType::LABEL : ResultType::CONSTANT,
		          (left.result_type == ResultType::LABEL) ? left.relocation_symbol : right.relocation_symbol);
}

ExprEval::Result ExprEval::subtraction(const Result& left, const Result& right)
{
	if (isConstMinusLabel(left, right) || !areSectionsCompatible(left, right))
	{
		throw AssemblerException("Invalid operand (" + m_assembler.getSymbol(left.relocation_symbol->getName()).getSection() +  " and " +
			m_assembler.getSymbol(right.relocation_symbol->getName()).getSection() + ") sections for '-'");
	}

	// Proveravamo da li je konstanta
	bool isConstant = left.relocation_symbol->getSection() == right.relocation_symbol->getSection();

	// Dohvatamo simbol koji predstavlja sekciju za konstante
	const auto* const_section = &m_assembler.getSymbol(CONST_SECTION);

	return Result(left.value - right.value,
				  isConstant ? ResultType::CONSTANT : left.result_type,
				  isConstant ? const_section        : left.relocation_symbol);
}

ExprEval::Result ExprEval::eval(const Result& left, const Result& right, const Token& op)
{
	if (op.getTokenType() == TokenType::PLUS)
		return addition(left, right);
	else if (op.getTokenType() == TokenType::MINUS)
		return subtraction(left, right);

	throw AssemblerException("Unsupported operation in ExprEval class");
}

ExprEval::Result::Result(int value, ResultType result_type, const Symbol* relocation_symbol)
	:value(value), result_type(result_type), relocation_symbol(relocation_symbol)
{

}
