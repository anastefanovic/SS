#include "assembler.h"
#include "codes.h"
#include "expr_eval.h"
#include "instructions.h"
#include "printer.h"
#include "utils.h"



Assembler::Assembler():
	lc(0)
{
	// Dodamo 
	symbol_table.push_back(Symbol(UNDEF_SECTION, 0, SymbolType::SECTION, UNDEF_SECTION, true));
	symbol_table.push_back(Symbol(CONST_SECTION, 0, SymbolType::SECTION, CONST_SECTION, true));
}

void Assembler::defineLabel(std::string label_name, bool is_defined)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	for (auto& symbol : symbol_table)
	{
		// simbol je definisan kada postoji u tabeli simbola I kada je vec naidjena njegova definicija
		if (symbol.getName() == label_name)
		{
			if (symbol.isDefined())
				throw AssemblerException("Label (" + label_name + ") is already defined!");

			symbol.setDefined(is_defined);
			symbol.setSection(current_section);
			symbol.setValue(lc);

			return;
		}
	}

	// Simbol ne postoji u tabeli simbola i dodajemo njegovu definiciju(nije ranije referenciran)
	symbol_table.push_back(Symbol(label_name, lc, SymbolType::LABEL, current_section, is_defined));
}

void Assembler::defineGlobalExtern(std::string label_name)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	for (auto& symbol : symbol_table)
	{
		if (symbol.getName() == label_name)
		{
			if (symbol.getSymbolType() != SymbolType::LABEL)
				throw AssemblerException("Symbol with name {" + label_name + "} is not a label!");

			symbol.setSymbolScope(SymbolScope::GLOBAL);

			return;
		}
	}

	// Ne znamo kojoj sekciji pripada, stoga nije ni definisan jos uvek
	symbol_table.push_back(Symbol(label_name, 0, SymbolType::LABEL, UNDEF_SECTION, false));
}

void Assembler::defineSection(std::string section_name)
{
	for (auto& symbol : symbol_table)
	{
		if (symbol.getName() == section_name)
		{
			throw AssemblerException("Cannot define section because symbol with name {" + section_name + "} already exists.");
		}
	}

	if (current_section != UNDEF_SECTION)
	{
		getSymbol(current_section).setValue(lc);
	}

	// Definisemo simbol koji predstavja sekciju, resetujemo Location counter na 0, jer obradjujemo novu sekciju
	// i trenutnu sekciju postavimo na novodefinisanu.
	symbol_table.push_back(Symbol(section_name, 0, SymbolType::SECTION, section_name, true));
	// definisemo u mapi ulaz za trenutnu sekciju
	relocation_table[section_name] = std::vector<Relocation>();
	current_section = section_name;
	lc = 0;
}

void Assembler::defineData(int size, std::string label_name)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	// Pronalazimo da li labela/simbol postoji
	int symbol_idx = -1;
	for (int i = 0; i < symbol_table.size(); ++i)
	{
		if (symbol_table[i].getName() == label_name)
		{
			symbol_idx = i;
			break;
		}
	}

	// Ako ne postoji, dodamo.
	if (symbol_idx == -1)
	{
		symbol_table.push_back(Symbol(label_name, 0, SymbolType::LABEL));
		symbol_idx = symbol_table.size() - 1;
	}

	// Ako nije definisano, dodamo referencu unapred
	if (!symbol_table[symbol_idx].isDefined())
	{
		symbol_table[symbol_idx].addForwardReference(ForwardReference(lc, current_section));
	}

	// Upisujemo vrednost simbola u memoriju.
	writeData(current_section, symbol_table[symbol_idx].getValue(), size);
	writeRelocation(current_section, lc, symbol_table[symbol_idx], RelocationType::ABSOLUTE);

	lc += size;
}


void Assembler::defineData(int data_size, int value)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	writeData(current_section, value, data_size);

	lc += data_size;
}

void Assembler::defineSkip(int size)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	// memoriju koju alociramo samo popunimo nulama.
	writeData(current_section, 0, size);

	lc += size;
}

void Assembler::equDefinition(std::string symbol_name, const std::vector<Token>& infix_expr)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	ExprEval expr_eval(*this);

	for (auto& symbol : symbol_table)
	{
		if (symbol_name == symbol.getName())
			throw AssemblerException("Symbol with name { " + symbol_name + "} is already defined!");
	}

	auto result = expr_eval.evaluateExpr(infix_expr);
	
	// Definisemo EQU simbol, zatim postavimo njegov relokacioni zapis, za slucaj da postoji
	Symbol equSymbol = Symbol(symbol_name, result.value, SymbolType::EQU, current_section, true);
	if (result.relocation_symbol->isDefined())
		equSymbol.setRelocationSymbol(result.relocation_symbol->getSection());
	else
		equSymbol.setRelocationSymbol(result.relocation_symbol->getName());

	symbol_table.push_back(equSymbol);
}

void Assembler::instructionDefinition(const Token& instruction, const Operand& op1, const Operand& op2)
{
	if (current_section.empty())
	{
		throw AssemblerException("Cannot define a symbol outside of a section!");
	}

	char c = InstructionsUtil::getInstructionSuffix(instruction);
	int op_size = (c == 'b') ? 1 : 2;
	int lc_increment = 0;

	if (!InstructionsUtil::isValid(instruction, op1, op2))
	{
		// prijaviti gresku
		throw AssemblerException("Invalid instruction");
	}

	data_table[current_section].push_back(InstructionsUtil::formatInstructionByte(instruction));
	// upisali smo instrukciju, uvecamo za jedan, sada LC pokazuje na deskriptor operanda prvog
	lc++;

	// Racunanje velicine operanda = Deskriptor + velicina od 1B ili 2B
	// Deskriptor sigurno imamo, dok preostala 2 ili 1B mozda
	// to zavisi od adresiranje, gde ukoliko je registarsko direktno/indirektno nemamo dodatne bajtove sem deskriptora
	// dok kod ostalih adresiranja imamo sigurno jos ta 2 bajta(ili 1 ukoliko je neposredno sa literalom)
	processInstructionOperand(op1, op_size);

	// Isto to i za op2
	processInstructionOperand(op2, op_size);
}

Symbol& Assembler::getSymbol(std::string symbol_name)
{
	for (auto& symbol : symbol_table)
	{
		if (symbol.getName() == symbol_name)
			return symbol;
	}

	// Simbol ne postoji, dodamo ga
	symbol_table.push_back(Symbol(symbol_name, 0, SymbolType::LABEL));

	return symbol_table[symbol_table.size() - 1];
}

void Assembler::resolveForwardReferences()
{
	for (auto& symbol : symbol_table)
	{
		if (symbol.getSymbolType() != SymbolType::LABEL || !symbol.isDefined())
			continue;

		auto forward_references = symbol.getForwardReferences();
		for (auto& ref : forward_references)
		{
			auto& section_rel_tbl = relocation_table[ref.section];

			for (int i = 0; i < section_rel_tbl.size(); ++i)
			{
				if (section_rel_tbl[i].getAddr() != ref.addr)
					continue;

				int value = readData(ref.section, ref.addr, 2);
				

				section_rel_tbl[i].setSymbolName(symbol.getSection());

				// Ukoliko je u pitanju bila PC relativna relokacija, a nakon prepravke simbol se nalazi unutar sekcije
				// gde i relokacija, nema potrebe za relokacijom, jer se pomeraj izmedju simbola i pc relativnog adresiranja
				// ne menja pri smestanju sekcija unutar memorije, stoga relokacija nije potrebna.
				if (section_rel_tbl[i].getType() == RelocationType::PC_RELATIVE && ref.section == symbol.getSection())
				{
					section_rel_tbl.erase(section_rel_tbl.begin() + i);

					// ukoliko je PC relativno, inicijalno je upisano na lokaciji vrednost_simbola - 2, medjutim, ovde znamo da je
					// simbol u istoj sekciji gde i PC, stoga nam ne treba relokacija, a stvarni pomeraj racunamo po principu
					// tako sto od value oduzmemo jos i offset na kojem se PC nalazi.
					value -= ref.addr;
				}

				// dodajemo vrednost koju smo upisali na tu lokaciju i vrednost simbola
				writeData(ref.section, ref.addr, value + symbol.getValue(), 2);
				
				break; // break (int i = 0; i < section_rel_tbl.size(); ++i)
			}
		} // end of for (auto& ref : forward_references)
	} // end of for (auto& symbol : symbol_table)
}

void Assembler::print(FILE* handle)
{
	Printer::printSymbolTable(symbol_table, handle);
	Printer::print(relocation_table, data_table, handle);
}

void Assembler::endDefinition()
{
	if (current_section != UNDEF_SECTION)
	{
		getSymbol(current_section).setValue(lc);
	}

	for (auto& symbol : symbol_table)
	{
		if (symbol.getSymbolType() != SymbolType::LABEL)
			continue;

		if (symbol.getSection() == UNDEF_SECTION)
			symbol.setSymbolScope(SymbolScope::GLOBAL);
	}
}

void Assembler::writeData(std::string section_name, int value, int data_size)
{
	int mask = 0x00ff;
	for (int i = 0; i < data_size; ++i)
	{
		data_table[section_name].push_back(value & mask);
		value >>= 8;
	}
}

void Assembler::writeData(std::string section_name, int position, int value, int data_size)
{
	int mask = 0x00ff;
	for (int i = 0; i < data_size; ++i)
	{
		data_table[section_name][position + i] = (value & mask);
		value >>= 8;
	}
}

int Assembler::readData(std::string section_name, int position, int data_size)
{
	int value = 0;
	for (int i = 0; i < data_size; ++i)
	{
		value |= int(data_table[section_name][position + i]) << (i * 8);
	}

	return value;
}

/// <summary>
/// Funkcija koja radi relokaciju samo u slucaju kada je u pitanju labela
/// i kada je u pitanju EQU Simbol koji u sebi ima relokacioni simbol.
/// </summary>
/// <param name="section_name">naziv sekcije gde se dodaje relokacija</param>
/// <param name="lc">offset unutar sekcije gde se dodaje relokacija</param>
/// <param name="symbol">simbol za kojeg je relokacija vezana</param>
/// <param name="reloc_type">tip relokacije(apsolutni, pc relativni)</param>
void Assembler::writeRelocation(std::string section_name, int lc, Symbol& symbol, RelocationType reloc_type)
{
	if (symbol.getSymbolType() == SymbolType::LABEL)
	{
		// Ukoliko simbol nije defininsan, radi se relokacija nad njim (upise se na mesto gde se pojavljuje 0),a relokacioni zapis
		// zahteva da se na to mesto doda njegova adresau.
		// Ukoliko je simbol definisan, na njegovo mesto se njegov offset u odnosu na pocetak sekcije u kojoj je definisan, a relokacija
		// zahteva da se na to mesto doda adresa sekcije u kojoj je simbol definisan.
		std::string relocation_symbol_name = symbol.isDefined() ? symbol.getSection() : symbol.getName();
		relocation_table[section_name].push_back(Relocation(lc, reloc_type, relocation_symbol_name));
	}
	else if (symbol.getSymbolType() == SymbolType::EQU)
	{
		// Ukoliko EQU ima postavljen relokacioni simbol, onda treba i za njega relokaciju da pravimo
		if (symbol.getRelocationSymbol() != CONST_SECTION)
			relocation_table[section_name].push_back(Relocation(lc, reloc_type, symbol.getRelocationSymbol()));
	}
}

void Assembler::processInstructionOperand(const Operand& op, int op_size)
{
	int lc_increment = 0;

	if (InstructionsUtil::isRegisterAddressing(op))
	{
		processRegisterAddressing(op, op_size);

		lc_increment += 1;
	}
	else if (op.getAddressingType() == AddressingType::IMMEDIATE)
	{
		processImmediateAddressing(op, op_size);

		// inkrement nam je lc_increment + operand_deskriptor + velicina operanda(2B ili 1B);
		lc_increment += 1 + op_size;
	}
	else if (op.getAddressingType() == AddressingType::REGINDIRECT_DISPL)
	{
		// deskriptor + velicina operanda
		lc_increment += 1 + 2;

		// Upisujemo deskriptor operanda
		writeData(current_section, Codes::formatOperandDescriptor(op), 1);
		//writeData(current_section, (getAddressingCode(op.getAddressingType()) << 5) | register_code, 1);

		int valueToWrite = 0;
		if (op.getFirstParam().getTokenType() == TokenType::LITERAL)
		{
			valueToWrite = utils::stoi(op.getFirstParam().getValue());
		}
		else // kada je LABEL-a/Simbol
		{
			Symbol& symbol = getSymbol(op.getFirstParam().getValue());

			// simbol(a) - (pc + pomeraj) gde je pomeraj u odnosu na mesto relokacije, 
			// relokacija se radi na mestu lc + 1, medjutim, nakon sto procesor procita pomeraj
			// njegov PC ce se pomeriti za dva mesta (lc + 3), a pomeraj se dodaje na tu vrednost PC-a
			// i tako se dobija adresa sa koje se cita vrednost.
			if (isPcReg(op.getSecondParam()))
			{
				if (current_section != symbol.getSection())
					valueToWrite = symbol.getValue() - 2;
				else
					valueToWrite = symbol.getValue() - (lc + 3);
			}
			else
			{
				valueToWrite = symbol.getValue();
			}

			// Za PC relativno, ukoliko je simbol iz iste sekcije gde se nalazi i PC
			// nema potrebe za relokacijom, jer je pomeraj simbola u odnosu na PC
			// uvek fiksan(const)
			if (current_section != symbol.getSection())
			{
				writeRelocation(current_section,
					lc + 1, // lc pokazuje na operand deskriptor, a na +1 od toga treba da se nadje pomeraj
					symbol,
					isPcReg(op.getSecondParam()) ? RelocationType::PC_RELATIVE
					: RelocationType::ABSOLUTE);
			}

			writeData(current_section, valueToWrite, 2);
		}
	}else if (op.getAddressingType() == AddressingType::MEMDIR)
	{
		// deskriptor + velicina operanda
		lc_increment += 1 + 2;

		// Upisujemo deskriptor operanda
		writeData(current_section, Codes::formatOperandDescriptor(op), 1);

		int valueToWrite = 0;
		if (op.getFirstParam().getTokenType() == TokenType::LITERAL)
		{
			valueToWrite = utils::stoi(op.getFirstParam().getValue());
		}
		else
		{
			Symbol& symbol = getSymbol(op.getFirstParam().getValue());

			valueToWrite = symbol.getValue();

			writeRelocation(current_section, lc + 1, symbol, RelocationType::ABSOLUTE);
		}

		writeData(current_section, valueToWrite, 2);
	}

	lc += lc_increment;
}

void Assembler::processRegisterAddressing(const Operand& op, int op_size)
{
	writeData(current_section, Codes::formatOperandDescriptor(op), 1);
}

void Assembler::processImmediateAddressing(const Operand& op, int op_size)
{
	// upisali smo da je u pitanju neposredno adresiranje
	writeData(current_section, Codes::formatOperandDescriptor(op), 1);

	int valueToWrite = 0;
	// Ukoliko je u pitanju EQU ili Labela, moramo dodati i relokaciju
	if (op.getFirstParam().getTokenType() != TokenType::LITERAL)
	{
		if (op_size == 1)
			throw AssemblerException("Expected operand size is 1B, but actual size is 2B");
		Symbol& symbol = getSymbol(op.getFirstParam().getValue());

		valueToWrite = symbol.getValue();

		// Relokacija se radi na mestu lc + 1.
		writeRelocation(current_section, lc + 1, symbol);
	}
	else
	{
		valueToWrite = utils::stoi(op.getFirstParam().getValue());
	}

	writeData(current_section, valueToWrite, op_size);
}

bool Assembler::isPcReg(const Token& reg)
{
	if (reg.getTokenType() != TokenType::REGISTER)
		return false;

	std::string reg_name = reg.getValue();
	
	// Izbacimo sufiks h/l iz imena registra.
	if (reg_name[reg_name.length() - 1] == 'h' || reg_name[reg_name.length() - 1] == 'l')
		reg_name = reg_name.substr(0, reg_name.length() - 1);

	return reg_name == "%pc" || reg_name == "%r7";
}
