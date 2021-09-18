#include "relocation.h"

Relocation::Relocation(int addr, RelocationType type, std::string symbol_name)
	: m_addr(addr),
	  m_type(type),
	  m_symbol_name(symbol_name)
{

}

int Relocation::getAddr() const
{
	return m_addr;
}

RelocationType Relocation::getType() const
{
	return m_type;
}

std::string Relocation::getSymbolName() const
{
	return m_symbol_name;
}

void Relocation::setSymbolName(const std::string& sym_name)
{
	m_symbol_name = sym_name;
}
