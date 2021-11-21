#pragma once
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <deque>
#include <map>

class Disassembly
{
	public:
		// Constructors
		Disassembly();
		~Disassembly();

		// Takes in Data to Log Disassembly
		void logDisassembly(uint8_t& opcode, uint8_t& operands[2], uint16_t& addr);

		// Formatting Functions
		void formatDisassemblyLine();
		inline std::string formatHex(uint8_t& n);
		

	private:
		// 6502 Instruction Variables
		struct NESDisassembly
		{
			std::string pcaddr;
			std::string opcode;
			std::string operand[2];
			std::string addrmode;
			std::string instruction;
			std::string line;
		};
		NESDisassembly nDisassembly;


		// Structure of OpLookup Table Disassembly
		struct NESInstructionDisassembly
		{
			std::string opcode;
			std::string operand[2];
			std::string addrmode;
			std::string instruction;
		};

		// Disassembly Queue
		std::deque<std::string> disassemby_queue;

		// Lookup tables 
		std::map<int,std::string> hex_lookup;
		std::vector<NESInstructionDisassembly> nes_opLookup;

};