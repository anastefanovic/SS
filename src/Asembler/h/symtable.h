#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <string>
#include <list>

static const std::string UNDEF_SECTION = "*UNDEF*";
static const std::string CONST_SECTION = "*ABS*";

struct ForwardReference
{
	int addr;
	std::string section;

	ForwardReference(int addr, std::string section) : addr(addr), section(section) {}
};

enum class SymbolType { EQU, LABEL, SECTION };

enum class SymbolScope {GLOBAL, LOCAL};
class Symbol
{
public:
	Symbol(std::string name, int value, SymbolType type, std::string section = UNDEF_SECTION, bool is_defined = false);

	bool isDefined() const;
	std::string getName() const;
	int getValue() const;
	SymbolType getSymbolType() const;
	SymbolScope getSymbolScope() const;
	std::string getSection() const;
	std::string getRelocationSymbol() const;

	void setValue(int value);
	void setDefined(bool defined);
	void setSection(std::string section);
	void setSymbolScope(SymbolScope symbol_scope);
	void setRelocationSymbol(std::string relocation_symbol);
	

	std::list<ForwardReference> getForwardReferences();

	void addForwardReference(ForwardReference forward_reference);
private:
	SymbolType m_type;
	bool m_is_defined;
	std::string m_name;
	std::string m_section;
	int m_value;
	SymbolScope m_symbol_scope;

	// Used by EQU Symbol only.
	std::string m_relocation_symbol;
	std::list<ForwardReference> m_forward_references;
};

//class SymbolTable
//{
//	
//};












#endif