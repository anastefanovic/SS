#include <cstdio>
#include <iostream>
#include <map>

#include "printer.h"

void Printer::printSymbolTable(const std::vector<Symbol>& symbol_table, FILE* handle)
{
	fprintf(handle, "#symbol table\n");
	fprintf(handle, "#%10s %10s %10s %10s %10s\n", "name", "section", "value", "scope", "no.");

	for (int i = 0; i < symbol_table.size(); ++i)
	{
		auto& symbol = symbol_table[i];

		if (symbol.getName() == CONST_SECTION)
			continue;

		fprintf(handle, "%11s %10s %10d %10s %10d\n", symbol.getName().c_str(),
			symbol.getSection().c_str(),
			symbol.getValue(),
			symbol.getSymbolScope() == SymbolScope::GLOBAL ? "global" : "local",
			i);
	}
}

void Printer::printRelocationTable(const std::vector<Relocation>& relocation_table, FILE* handle)
{
	using namespace std;

	fprintf(handle, "#%15s %15s %10s\n", "Address", "Type", "Symbol");
	for (auto& reloc : relocation_table)
	{
		fprintf(handle, "#%15d %15s %10s\n", reloc.getAddr(), reloc.getType() == RelocationType::ABSOLUTE ? "ABSOLUTE" : "PC_RELATIVE", reloc.getSymbolName().c_str());
	}
}

void Printer::printSection(const std::string& section_name, const std::vector<Relocation> relocation_table, const std::vector<unsigned char> data_table, FILE* handle)
{
	using namespace std;

	fprintf(handle, "#%15s\n", section_name.c_str());
	printRelocationTable(relocation_table, handle);
	printDataTable(data_table, handle);
}


void Printer::print(const RelocationTables& reloc_tables, const DataTables& data_tables, FILE* handle)
{
	for (auto& pair : reloc_tables)
	{
		fprintf(handle, "#%s\n", pair.first.c_str());
		printRelocationTable(pair.second, handle);
	}

	for (auto& pair : data_tables)
	{
		fprintf(handle, "#%s\n", pair.first.c_str());
		printDataTable(pair.second, handle);
	}
}

void Printer::printDataTable(const std::vector<unsigned char> data_table, FILE* handle)
{
	using namespace std;

	int size = data_table.size();
	for (int i = 0; i < size;)
	{
		for (int j = 0; (i < size) && (j < 10); ++j, ++i)
		{
			fprintf(handle, "%02X ", data_table[i]);
		}

		fprintf(handle, "\n");
	}
}
