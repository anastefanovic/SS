#include "symtable.h"



Symbol::Symbol(std::string name, int value, SymbolType type, std::string section, bool is_defined)
	: m_name(name), 
	  m_section(section),
	  m_value(value),
	  m_type(type),
	  m_is_defined(is_defined),
	  m_symbol_scope(SymbolScope::LOCAL)
{

}

bool Symbol::isDefined() const
{
	return m_is_defined;
}

std::string Symbol::getName() const
{
	return m_name;
}

int Symbol::getValue() const
{
	return m_value;
}

SymbolType Symbol::getSymbolType() const
{
	return m_type;
}

SymbolScope Symbol::getSymbolScope() const
{
	return m_symbol_scope;
}

std::string Symbol::getSection() const
{
	return m_section;
}

std::string Symbol::getRelocationSymbol() const
{
	return m_relocation_symbol;
}

void Symbol::setValue(int value)
{
	m_value = value;
}

void Symbol::setDefined(bool defined)
{
	m_is_defined = defined;
}

void Symbol::setSection(std::string section)
{
	m_section = section;
}

void Symbol::setSymbolScope(SymbolScope symbol_scope)
{
	m_symbol_scope = symbol_scope;
}

void Symbol::setRelocationSymbol(std::string relocation_symbol)
{
	m_relocation_symbol = relocation_symbol;
}

std::list<ForwardReference> Symbol::getForwardReferences()
{
	return m_forward_references;
}

void Symbol::addForwardReference(ForwardReference forward_reference)
{
	m_forward_references.push_back(forward_reference);
}