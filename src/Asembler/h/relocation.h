#ifndef _RELOCATION_H_
#define _RELOCATION_H_

#include <string>

enum class RelocationType
{
	ABSOLUTE, PC_RELATIVE
};

class Relocation
{
public:
	Relocation(int addr, RelocationType type, std::string symbol_name);

	int getAddr() const;
	RelocationType getType() const;
	std::string getSymbolName() const;

	void setSymbolName(const std::string& sym_name);
private:
	int m_addr;
	RelocationType m_type;
	std::string m_symbol_name;
};


class RelocationUtil
{

};
#endif // !_RELOCATION_H_
