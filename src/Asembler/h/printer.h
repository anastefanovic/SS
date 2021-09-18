#ifndef _PRINTER_H_
#define _PRINTER_H_

#include "symtable.h"
#include "relocation.h"
#include <vector>


using RelocationTables = std::map<std::string, std::vector<Relocation>>;
using DataTables = std::map<std::string, std::vector<unsigned char>>;
class Printer
{
private:
	Printer() {}
	Printer(const Printer&) = delete;
	Printer& operator=(const Printer&) = delete;

public:
	static void printSymbolTable(const std::vector<Symbol>& symbol_table, FILE* handle);
	static void printRelocationTable(const std::vector<Relocation>& relocation_table, FILE* handle);
	static void printSection(const std::string& section_name, const std::vector<Relocation> relocation_table, const std::vector<unsigned char> data_table, FILE* handle);
	static void print(const RelocationTables& reloc_tables, const DataTables& data_tables, FILE* handle);
	static void printDataTable(const std::vector<unsigned char> data_table, FILE* handle);
};
#endif // !_PRINTER_H_
