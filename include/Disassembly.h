#pragma once
#include <sstream>
#include <array>
#include <string>
#include <deque>
#include <unordered_map>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <bitset>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <windows.h>
#include <algorithm>

#define DEQUE_SIZE 18
#define HEX_CHAR_SIZE 3
#define INSTR_CHAR_SIZE 5
#define ADDRMODE_CHAR_SIZE 4

class Disassembly
{
	public:
		// Constructors
		Disassembly();
		~Disassembly();

		// Takes in Data to Log Disassembly
		void logDisassembly(uint8_t opcode, uint8_t operands[2], int numOpers, uint16_t addr);

		// Hex Formatter - Integer to String Hex Values
		char* formatHex(uint8_t n);		

		// Get Functions
		std::deque <std::string> getDisassemblyQueue();
		const char* getInstruction();
		const char* getAddrMode();
		const char* getOps();
		const char* getOpcode();

	private:
		// NES Disassembly 
		struct NESDisassembly
		{
			std::string pcaddr;
			char* pc;
			char* opcode;
			char* operand[2];
			int num_operands = 0;
			char* instruction;
			char* addrmode;
			char* op_line;
			std::string line;
		};

		// NES Opcode 
		struct NESOpcodeTable
		{
			char instruction[INSTR_CHAR_SIZE];
			char addrmode[ADDRMODE_CHAR_SIZE];
		};

		// Hex Lookup
		struct HexTable
		{ 
			char hex_string[HEX_CHAR_SIZE]; 
		};		
	

	private:
		HexTable hex_lookup[256];					// Hex Lookup - (8-bit for now)
		NESOpcodeTable disassemOp_lookup[256];		// Opcode Lookup
		NESDisassembly nDisassembly;				// Disassembly Elements
		std::deque <std::string> disassemby_queue;	// Disassembly Queue
		std::string addr;							// Addr Mode for Disassembly Line Format

		// Logging Functions
		void disassemblyQueueHandler();					// Handles Disassembly Queue
		void formatDisassemblyLine(uint8_t &opcode);	// Formats Disassembly Line for Queue
		void writeLog();

};