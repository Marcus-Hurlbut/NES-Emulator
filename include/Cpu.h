#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <deque>

//#include "Log.h"

class Bus;

class Cpu
{
    public:
        // Constructors
        Cpu();
        ~Cpu();

    public:
        // Link Communication to the Bus
        Bus *bus = nullptr;
        void ConnectBus(Bus *n) { 
            bus = n;
        }

        // CPU read & write 
        inline uint8_t read(uint16_t a);
        inline void write(uint16_t a, uint8_t d);

        // CPU Controller variables
        struct Controller 
        {
            uint8_t fetched = 0x00;     // Input value -> ALU, fetched address
            uint16_t temp = 0x0000;     // temporary variable
            uint16_t mem_addr = 0x0000; // All used memory addresses
            uint16_t rel_addr = 0x0000; // Represents absolute address after a branch
            uint8_t opcode = 0x00;      // Opcode instruction 
            uint8_t cycles = 0;         // CPU cycle counter
        };

        // CPU registers
        struct Registers 
        {
            uint16_t PC = 0x0000;       // Program Counter
            uint8_t S = 0x00;           // Stack pointer
            uint8_t A = 0x00;           // Accumulator
            uint8_t X = 0x00;           // X-index
            uint8_t Y = 0x00;           // Y-index
            uint8_t P = 0x00;           // Status Register
        };

        // CPU objects
        Controller con;        // Controller object
        Registers reg;       // Register object

        // Manipulate P register flag bits
        enum StatusFlags
        {
            C = (1 << 0), // Set Carry
            Z = (1 << 1), // Set Zero
            I = (1 << 2), // Set Interrupt disable
            D = (1 << 3), // Set Decimal mode
            B = (1 << 4), // No CPU effect, see: 'the B flag'
            U = (1 << 5), // Unused (always 1)
            V = (1 << 6), // Set Overflow
            N = (1 << 7)  // Set Negative
        };

    private:
        uint8_t getStatusRegister(StatusFlags flags);               // Flag
        void setStatusRegister(StatusFlags flags, bool mode);       // Mode: set or clear P bit values

    public:
        // CPU Interrupts
        void reset();       // reset
        void irq();         // Interrupt Request
        void nmi();         // Non-maskable Interrupt

        // CPU executor & fetcher
        uint16_t tick();                   // Execute instruction 
        inline uint8_t fetchInstruction();       // Fetch 
        uint8_t transferDMA(uint8_t &addr_offset, uint8_t &dma_addr);


    private:
        // CPU Addressing Modes
        uint8_t Addr_IMP();	   // Implicit / Implied
        uint8_t Addr_ACC();    // Accumulator
        uint8_t Addr_IMM();	   // Immediate
        uint8_t Addr_ZP0();	   // Zero Page
        uint8_t Addr_ZPX();    // Zero Page, X
        uint8_t Addr_ZPY();    // Zero Page, Y	
        uint8_t Addr_REL();    // Relative
        uint8_t Addr_ABS();	   // Absolute
        uint8_t Addr_ABX();	   // Absolute, X
        uint8_t Addr_ABY();	   // Absolute, Y
        uint8_t Addr_IND();	   // Indirect
        uint8_t Addr_INX();	   // Indirect Indexed X
        uint8_t Addr_INY();    // Indirect Indexed Y

        // Legal Instructions
        uint8_t ADC();      uint8_t AND();      uint8_t ASL();      uint8_t BCC();      uint8_t BCS();      uint8_t BEQ();      uint8_t BIT();    
        uint8_t BMI();      uint8_t BNE();      uint8_t BPL();      uint8_t BRK();      uint8_t BVC();      uint8_t BVS();      uint8_t CLC();
        uint8_t CLD();      uint8_t CLI();      uint8_t CLV();      uint8_t CMP();      uint8_t CPX();      uint8_t CPY();      uint8_t DEC();      
        uint8_t DEX();      uint8_t DEY();      uint8_t EOR();      uint8_t INC();      uint8_t INX();      uint8_t INY();      uint8_t JMP();
        uint8_t JSR();      uint8_t LDA();      uint8_t LDX();      uint8_t LDY();      uint8_t LSR();      uint8_t NOP();      uint8_t ORA();      
        uint8_t PHA();      uint8_t PHP();      uint8_t PLA();      uint8_t PLP();      uint8_t ROL();      uint8_t ROR();      uint8_t RTI();
        uint8_t RTS();      uint8_t SBC();      uint8_t SEC();      uint8_t SED();      uint8_t SEI();      uint8_t STA();      uint8_t STX();     
        uint8_t STY();      uint8_t TAX();      uint8_t TAY();      uint8_t TSX();      uint8_t TXA();      uint8_t TXS();      uint8_t TYA();
    public:    
        // Illegal Instructions
        uint8_t XXX();      
        uint8_t LAX();      uint8_t AAC();      uint8_t AAX();      uint8_t AAR();      uint8_t ASR();
        uint8_t ATX();      uint8_t AXA();      uint8_t AXS();      uint8_t DCP();      uint8_t DOP();
        uint8_t ISC();      uint8_t KIL();      uint8_t LAR();      uint8_t RLA();      uint8_t RRA();
        uint8_t SLO();      uint8_t SRE();      uint8_t SXA();      uint8_t SYA();      uint8_t TOP();
        uint8_t XXA();      uint8_t XAS();
        //uint8_t AAR();
    public:
        // Opcode Lookup Table
        struct OpLookup
        {   
            uint8_t op_opcode;                             // Opcode
            uint8_t op_cycles;                         // # of cycles
            uint8_t (Cpu::*op_instruction)();    // Instruction
            std::string op_sInstruction;
            uint8_t (Cpu::*op_addressmode)();    // Address Mode
            std::string addrMode;
        };

        // Instruction Opcode Table
        std::vector<OpLookup> op_lookup;

    public:
        // Disassembly and Log Functions

        struct Disassembly
        {
            std::string mem_location;
            std::string get_instruction;
            std::string get_addrmode;
            uint8_t get_opcode;
            std::string get_opcode_instruction[2];
            std::deque<std::string> disassembly; 
        };
        Disassembly d;

        // Disassembly Log dimensions
        int disassemblySize = 0;
        int disCount = 1;
        int max_size = 16;
        std::string disassembly_line;

        std::string thisDis = "";
        bool newInstruction = false;

        // Disassembly Opcode hex helpers
        std::stringstream ss;
        std::stringstream ss1;
        std::stringstream ss2;

        inline void setDisassemblyLog(std::string &disassembly);
        inline void setDisassemblyOp(uint8_t op1, uint8_t op2, bool multiple_op);
        bool renderDisassembly = false;

};