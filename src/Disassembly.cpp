#include "Disassembly.h"

Disassembly::Disassembly()
{
	// Initialize Hex Lookup
	HexTable h [256] =
	{
		{"00"},	{"01"},	{"02"},	{"03"},	{"04"}, {"05"}, {"06"}, {"07"}, {"08"}, {"09"}, {"0A"}, {"0B"}, {"0C"}, {"0D"}, {"0E"}, {"0F"},
		{"10"}, {"11"}, {"12"}, {"13"}, {"14"}, {"15"}, {"16"}, {"17"}, {"18"}, {"19"}, {"1A"}, {"1B"}, {"1C"}, {"1D"}, {"1E"}, {"1F"},
		{"20"}, {"21"}, {"22"}, {"23"}, {"24"}, {"25"}, {"26"}, {"27"}, {"28"}, {"29"}, {"2A"}, {"2B"}, {"2C"}, {"2D"}, {"2E"}, {"2F"},
		{"30"}, {"31"}, {"32"}, {"33"}, {"34"}, {"35"}, {"36"}, {"37"}, {"38"}, {"39"}, {"3A"}, {"3B"}, {"3C"}, {"3D"}, {"3E"}, {"3F"},
		{"40"}, {"41"}, {"42"}, {"43"}, {"44"}, {"45"}, {"46"}, {"47"}, {"48"}, {"49"}, {"4A"}, {"4B"}, {"4C"}, {"4D"}, {"4E"}, {"4F"},
		{"50"}, {"51"}, {"52"}, {"53"}, {"54"}, {"55"}, {"56"}, {"57"}, {"58"}, {"59"}, {"5A"}, {"5B"}, {"5C"}, {"5D"}, {"5E"}, {"5F"},
		{"60"}, {"61"}, {"62"}, {"63"}, {"64"}, {"65"}, {"66"}, {"67"}, {"68"}, {"69"}, {"6A"}, {"6B"}, {"6C"}, {"6D"}, {"6E"}, {"6F"},
		{"70"}, {"71"}, {"72"}, {"73"}, {"74"}, {"75"}, {"76"}, {"77"}, {"78"}, {"79"}, {"7A"}, {"7B"}, {"7C"}, {"7D"}, {"7E"}, {"7F"},
		{"80"}, {"81"}, {"82"}, {"83"}, {"84"}, {"85"}, {"86"}, {"87"}, {"88"}, {"89"}, {"8A"}, {"8B"}, {"8C"}, {"8D"}, {"8E"}, {"8F"},
		{"90"}, {"91"}, {"92"}, {"93"}, {"94"}, {"95"}, {"96"}, {"97"}, {"98"}, {"99"}, {"9A"}, {"9B"}, {"9C"}, {"9D"}, {"9E"}, {"9F"},
		{"A0"}, {"A1"}, {"A2"}, {"A3"}, {"A4"}, {"A5"}, {"A6"}, {"A7"}, {"A8"}, {"A9"}, {"AA"}, {"AB"}, {"AC"}, {"AD"}, {"AE"}, {"AF"},
		{"B0"}, {"B1"}, {"B2"}, {"B3"}, {"B4"}, {"B5"}, {"B6"}, {"B7"}, {"B8"}, {"B9"}, {"BA"}, {"BB"}, {"BC"}, {"BD"}, {"BE"}, {"BF"},
		{"C0"}, {"C1"}, {"C2"}, {"C3"}, {"C4"}, {"C5"}, {"C6"}, {"C7"}, {"C8"}, {"C9"}, {"CA"}, {"CB"}, {"CC"}, {"CD"}, {"CE"}, {"CF"},
		{"D0"}, {"D1"}, {"D2"}, {"D3"}, {"D4"}, {"D5"}, {"D6"}, {"D7"}, {"D8"}, {"D9"}, {"DA"}, {"DB"}, {"DC"}, {"DD"}, {"DE"}, {"DF"},
		{"E0"}, {"E1"}, {"E2"}, {"E3"}, {"E4"}, {"E5"}, {"E6"}, {"E7"}, {"E8"}, {"E9"}, {"EA"}, {"EB"}, {"EC"}, {"ED"}, {"EE"}, {"EF"},
		{"F0"}, {"F1"}, {"F2"}, {"F3"}, {"F4"}, {"F5"}, {"F6"}, {"F7"}, {"F8"}, {"F9"}, {"FA"}, {"FB"}, {"FC"}, {"FD"}, {"FE"}, {"FF"}
	};
	memcpy(&hex_lookup, &h, sizeof(h));

	// Initialize Opcode Disassmebly Lookup 
	NESOpcodeTable o [256] =
    {
        // 0x0X
        {"BRK", "IMP"},{"ORA", "INX"},{"XXX", "XXX"},{"*SLO", "INX"},{"*NOP", "XXX"},{"ORA", "ZP0"},{"ASL", "ZP0"},{"*SLO", "ZP0"}, 
        {"PHP", "IMP"},{"ORA", "IMM"},{"ASL", "ACC"},{"*AAC", "IMM"},{"*NOP", "XXX"},{"ORA", "ABS"},{"ASL", "ABS"},{"*SLO", "ABS"},

        // 0x1X
        {"BPL", "REL"},{"ORA", "INY"},{"XXX", "XXX"},{"*SLO", "INY"},{"*NOP", "XXX"},{"ORA", "ZPX"},{"ASL", "ZPX"},{"*SLO", "ZPX"},
        {"CLC", "IMP"},{"ORA", "ABY"},{"*NOP", "XXX"},{"*SLO", "ABY"},{"*NOP", "XXX"},{"ORA", "ABX"},{"ASL", "ABX"},{"*SLO", "ABX"},

        // 0x2X
        {"JSR", "ABS"},{"AND", "INX"},{"XXX", "XXX"},{"*RLA", "INX"},{"BIT", "ZP0"},{"AND", "ZP0"},{"ROL", "ZP0"},{"*RLA", "ZP0"},
        {"PLP", "IMP"},{"AND", "IMM"},{"ROL", "ACC"},{"*AAC", "IMM"},{"BIT", "ABS"},{"AND", "ABS"},{"ROL", "ABS"},{"*RLA", "ABS"},

        // 0x3X
        {"BMI", "REL"},{"AND", "INY"},{"XXX", "XXX"},{"*RLA", "INY"},{"*NOP", "XXX"},{"AND", "ZPX"},{"ROL", "ZPX"},{"*RLA", "ZPX"},
        {"SEC", "IMP"},{"AND", "ABY"},{"*NOP", "XXX"},{"*RLA", "ABY"},{"*NOP", "XXX"},{"AND", "ABX"},{"ROL", "ABX"},{"*RLA", "ABX"},

        // 0x4X
        {"RTI", "IMP"},{"EOR", "INX"},{"XXX", "XXX"},{"*SRE", "INX"},{"*NOP", "XXX"},{"EOR", "ZP0"},{"LSR", "ZP0"},{"*SRE", "ZP0"},
        {"PHA", "IMP"},{"EOR", "IMM"},{"LSR", "ACC"},{"XXX", "XXX"},{"JMP", "ABS"},{"EOR", "ABS"},{"LSR", "ABS"},{"*SRE", "ABS"},

        // 0x5X
        {"BVC", "REL"},{"EOR", "INY"},{"XXX", "XXX"},{"*SRE", "INY"},{"*NOP", "XXX"},{"EOR", "ZPX"},{"LSR", "ZPX"},{"*SRE", "ZPX"},
        {"CLI", "IMP"},{"EOR", "ABY"},{"*NOP", "XXX"},{"*SRE", "ABY"},{"*NOP", "XXX"},{"EOR", "ABX"},{"LSR", "ABX"},{"*SRE", "ABX"},

        // 0x6X
        {"RTS", "IMP"},{"ADC", "INX"},{"XXX", "XXX"},{"*RRA", "INX"},{"*NOP", "XXX"},{"ADC", "ZP0"},{"ROR", "ZP0"},{"*RRA", "ZP0"},
        {"PLA", "IMP"},{"ADC", "IMM"},{"ROR", "ACC"},{"*AAR", "IMM"},{"JMP", "IND"},{"ADC", "ABS"},{"ROR", "ABS"},{"*RRA", "ABS"},

        // 0x7X
        {"BVS", "REL"},{"ADC", "INY"},{"XXX", "XXX"},{"*RRA","INY"},{"*NOP", "XXX"},{"ADC", "ZPX"},{"ROR", "ZPX"},{"*RRA", "ZPX"},
        {"SEI", "IMP"},{"ADC", "ABY"},{"*NOP", "XXX"},{"*RRA", "ABY"},{"*NOP", "XXX"},{"ADC", "ABX"},{"ROR", "ABX"},{"*RRA", "ABX"},

		// 0x8X
        {"*NOP", "XXX"},{"STA", "INX"},{"XXX", "XXX"},{"*AAX", "INX"},{"STY", "ZP0"},{"STA", "ZP0"},{"STX", "ZP0"},{"*AAX", "ZP0"},
        {"DEY", "IMP"},{"XXX", "XXX"},{"TXA", "IMP"},{"XXX", "XXX"},{"STY", "ABS"},{"STA", "ABS"},{"STX", "ABS"},{"*AAX", "INX"},

		// 0x9X
        {"BCC", "REL"},{"STA", "INY"},{"XXX", "XXX"},{"XXX", "XXX"},{"STY", "ZPX"},{"STA", "ZPX"},{"STX", "ZPY"},{"*AAX", "ZPY"},
        {"TYA", "IMP"},{"STA", "ABY"},{"TXS", "IMP"},{"XXX", "XXX"},{"XXX", "XXX"},{"STA", "ABX"},{"XXX", "XXX"},{"XXX", "XXX"},

		// 0xAX
        {"LDY", "IMM"},{"LDA", "INX"},{"LDX", "IMM"},{"*LAX", "INX"},{"LDY", "ZP0"},{"LDA", "ZP0"},{"LDX", "ZP0"},{"*LAX", "ZP0"},
        {"TAY", "IMP"},{"LDA", "IMM"},{"TAX", "IMP"},{"XXX", "XXX"},{"LDY", "ABS"},{"LDA", "ABS"},{"LDX", "ABS"},{"*LAX", "ABS"},

		// 0xBX
        {"BCS", "REL"},{"LDA", "INY"},{"XXX", "XXX"},{"*LAX", "INY"},{"LDY", "ZPX"},{"LDA", "ZPX"},{"LDX", "ZPY"},{"*LAX", "ZPY"},
        {"CLV", "IMP"},{"LDA", "ABY"},{"TSX", "IMP"},{"XXX", "XXX"},{"LDY", "ABX"},{"LDA", "ABX"},{"LDX", "ABY"},{"*LAX", "ABY"},

		// 0xCX
        {"CPY", "IMM"},{"CMP", "INX"},{"XXX", "XXX"},{"*DCP", "INX"},{"CPY", "ZP0"},{"CMP", "ZP0"},{"DEC", "ZP0"},{"*DCP", "ZP0"},
		{"INY", "IMP"},{"CMP", "IMM"},{"DEX", "IMP"},{"XXX", "XXX"},{"CPY", "ABS"},{"CMP", "ABS"},{ "DEC", "ABS"},{"*DCP", "ABS"},

		// 0xDX
        {"BNE", "REL"},{"CMP", "INY"},{"XXX", "XXX"},{"*DCP", "INY"},{"*NOP", "XXX"},{"CMP", "ZPX"},{"DEC", "ZPX"},{"*DCP", "ZPX"},
        {"CLD", "IMP"},{"CMP", "ABY"},{"*NOP", "XXX"},{"*DCP", "ABY"},{"*NOP", "XXX"},{"CMP", "ABX"},{"DEC", "ABX"},{"*DCP", "ABX"},

		// 0xEX
        {"CPX", "IMM"},{"SBC", "INX"},{"XXX", "XXX"},{"*ISC", "INX"},{"CPX", "ZP0"},{"SBC", "ZP0"},{"INC", "ZP0"},{"*ISC", "ZP0"},
        {"INX", "IMP"},{"SBC", "IMM"},{"NOP", "IMP"},{"*SBC","IMM"},{"CPX", "ABS"},{"SBC", "ABS"},{"INC", "ABS"},{"*ISC", "ABS"},

		// 0xFX
        {"BEQ", "REL"},{"SBC", "INY"},{"XXX", "XXX"},{"*ISC", "INY"},{"*NOP", "XXX"},{"SBC", "ZPX"},{"INC", "ZPX"},{"*ISC", "ZPX"},
        {"SED", "IMP"},{"SBC", "ABY"},{"*NOP", "XXX"},{"*ISC", "XXX"},{"*NOP", "XXX"},{"SBC", "ABX"},{"INC", "ABX"},{"*ISC", "ABX"}
    };
	memcpy(&disassemOp_lookup, &o, sizeof(o));

	// Initialize variables
	disassemby_queue.resize(DEQUE_SIZE);
}

Disassembly::~Disassembly()
{

}

// Write to Log File
void Disassembly::writeLog()
{
	std::ofstream wLog("C:\\Users\\Thugs4Less\\Desktop\\Program Projects\\NES\\log\\disassembly.txt", std::ofstream::app); 
	wLog << nDisassembly.line; wLog << "\n";
}


// Convert integer to hex string
char* Disassembly::formatHex(uint8_t n)
{
	return hex_lookup[n].hex_string;
}


// Called from CPU to Log Disassembly
void Disassembly::logDisassembly(uint8_t opcode, uint8_t operands[2], int numOpers, uint16_t addr)
{

	// Get Hex String of Opcode
	nDisassembly.opcode = formatHex(opcode);

	// Get Hex String of Operands
	if (numOpers == 0)
	{
		nDisassembly.num_operands = numOpers;
	}
	else if (numOpers == 1)
	{
		nDisassembly.operand[0] = formatHex(operands[0]);
		nDisassembly.num_operands = numOpers;
	}
	else
	{
		nDisassembly.operand[0] = formatHex(operands[0]);
		nDisassembly.operand[1] = formatHex(operands[1]);
		nDisassembly.num_operands = numOpers;
	};

	// Get Hex String of Address (Program Counter)
	uint8_t hi = ((addr & 0xFF00) >> 8);
	uint8_t lo = (addr) & 0x00FF;
	nDisassembly.pcaddr = formatHex(hi);
	nDisassembly.pcaddr += formatHex(lo);
	nDisassembly.pc = const_cast<char*> (nDisassembly.pcaddr.c_str());

	// Lookup Instruction Disassembly from Opcode
	nDisassembly.instruction = disassemOp_lookup[opcode].instruction;
	nDisassembly.addrmode = disassemOp_lookup[opcode].addrmode;

	// Format disassembly line
	formatDisassemblyLine(opcode);

	// Add to Disassembly Queue
	disassemblyQueueHandler();

	// Write to Log -- uncomment to write diassembly to log file
	//writeLog();		

}

void Disassembly::formatDisassemblyLine(uint8_t &opcode)
{
	addr = nDisassembly.addrmode;
	nDisassembly.line.reserve(48);
	
	if (addr == "IMP")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ACC")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "IMM")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " #$";
		nDisassembly.line += nDisassembly.operand[0];
		nDisassembly.line += " [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ZP0")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[0];
		nDisassembly.line += " [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ZPX")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[0];
		nDisassembly.line += ",X [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ZPY")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[0];
		nDisassembly.line += ",Y [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "REL")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[1];	// High byte
		nDisassembly.line += nDisassembly.operand[0];	// Low byte
		nDisassembly.line += " [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ABS")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[1];	// High byte
		nDisassembly.line += nDisassembly.operand[0];	// Low byte
		nDisassembly.line += " [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ABX")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[1];	// High byte
		nDisassembly.line += nDisassembly.operand[0];	// Low byte
		nDisassembly.line += ",X [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "ABY")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " $";
		nDisassembly.line += nDisassembly.operand[1];	// High byte
		nDisassembly.line += nDisassembly.operand[0];	// Low byte
		nDisassembly.line += ",Y [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "IND")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " ($";
		nDisassembly.line += nDisassembly.operand[1];	// High byte
		nDisassembly.line += nDisassembly.operand[0];	// Low byte
		nDisassembly.line += ") [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "INX")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " ($";
		nDisassembly.line += nDisassembly.operand[0];
		nDisassembly.line += ",X) [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}
	else if (addr == "INY")
	{
		nDisassembly.line = nDisassembly.pc;
		nDisassembly.line += ": ";
		nDisassembly.line += nDisassembly.instruction;
		nDisassembly.line += " ($";
		nDisassembly.line += nDisassembly.operand[0];
		nDisassembly.line += ",Y) [";
		nDisassembly.line += nDisassembly.addrmode;
		nDisassembly.line += "]";
	}		
}

void Disassembly::disassemblyQueueHandler()
{
	// Pop Back of Queue
	if (disassemby_queue.size() >= DEQUE_SIZE)
	{
		disassemby_queue.pop_back();
	}
	// Push to Front of Queue
	disassemby_queue.push_front(nDisassembly.line);
}


// Get Instruction for Rendering
const char* Disassembly::getInstruction()
{
	return (const char*) nDisassembly.instruction;
}


// Get Address Mode for Rendering
const char* Disassembly::getAddrMode()
{
	return (const char*) nDisassembly.addrmode;
}


// Get Opcode and Operand Values for Rendering
const char* Disassembly::getOps()
{
	std::string ops_line;
	ops_line.reserve(8);
	ops_line = "     ";


	if (nDisassembly.num_operands == 1)
	{
		ops_line = nDisassembly.operand[0];
		ops_line += "  ";
	}
	else if (nDisassembly.num_operands == 2)
	{
		ops_line += nDisassembly.operand[0];
		ops_line += " ";
		ops_line += nDisassembly.operand[1];
	}

	nDisassembly.op_line = const_cast<char*> (ops_line.c_str());

	return (const char*) nDisassembly.op_line;
}


// Get Opcode for Rendering
const char* Disassembly::getOpcode()
{
	return (const char*)nDisassembly.opcode;
}


// Get element in Disassembly Queue for Rendering
std::deque <std::string> Disassembly::getDisassemblyQueue()
{
	return disassemby_queue;
}
