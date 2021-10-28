#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#include <stdio.h>
#include <stdint.h>
#include "Cpu.h"
#include "Bus.h"



Cpu::Cpu()
{
    op_lookup = 
    {
        {0x00, 7, &Cpu::BRK, "BRK", &Cpu::Addr_IMP, "IMP"},
        {0x01, 6, &Cpu::ORA, "ORA", &Cpu::Addr_INX, "INX"},
        {0x02, 2, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x03, 8, &Cpu::SLO, "*SLO", &Cpu::Addr_INX, "INX [XXX]"},
        {0x04, 3, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x05, 3, &Cpu::ORA, "ORA", &Cpu::Addr_ZP0, "ZP0"},
        {0x06, 5, &Cpu::ASL, "ASL", &Cpu::Addr_ZP0, "ZP0"},
        {0x07, 5, &Cpu::SLO, "*SLO", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0x08, 3, &Cpu::PHP, "PHP", &Cpu::Addr_IMP, "IMP"},
        {0x09, 2, &Cpu::ORA, "ORA", &Cpu::Addr_IMM, "IMM"},
        {0x0A, 2, &Cpu::ASL, "ASL", &Cpu::Addr_ACC, "ACC"},
        {0x0B, 2, &Cpu::AAC, "*AAC", &Cpu::Addr_IMM, "IMM"},
        {0x0C, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x0D, 4, &Cpu::ORA, "ORA", &Cpu::Addr_ABS, "ABS"},
        {0x0E, 6, &Cpu::ASL, "ASL", &Cpu::Addr_ABS, "ABS"},
        {0x0F, 6, &Cpu::SLO, "*SLO", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0x10, 2, &Cpu::BPL, "BPL", &Cpu::Addr_REL, "REL"},
        {0x11, 5, &Cpu::ORA, "ORA", &Cpu::Addr_INY, "INY"},
        {0x12, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x13, 8, &Cpu::SLO, "*SLO", &Cpu::Addr_INY, "INY [XXX]"},
        {0x14, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x15, 4, &Cpu::ORA, "ORA", &Cpu::Addr_ZPX, "ZPX"},
        {0x16, 6, &Cpu::ASL, "ASL", &Cpu::Addr_ZPX, "ZPX"},
        {0x17, 6, &Cpu::SLO, "*SLO", &Cpu::Addr_ZPX, "ZPX [XXX]"},
        {0x18, 2, &Cpu::CLC, "CLC", &Cpu::Addr_IMP, "IMP"},
        {0x19, 4, &Cpu::ORA, "ORA", &Cpu::Addr_ABY, "ABY"},
        {0x1A, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x1B, 7, &Cpu::SLO, "*SLO", &Cpu::Addr_ABY, "ABY [XXX]"},
        {0x1C, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x1D, 4, &Cpu::ORA, "ORA", &Cpu::Addr_ABX, "ABX"},
        {0x1E, 7, &Cpu::ASL, "ASL", &Cpu::Addr_ABX, "ABX"},
        {0x1F, 7, &Cpu::SLO, "*SLO", &Cpu::Addr_ABX, "ABX [XXX]"},

        {0x20, 6, &Cpu::JSR, "JSR", &Cpu::Addr_ABS, "ABS"},
        {0x21, 6, &Cpu::AND, "AND", &Cpu::Addr_INX, "INX"},
        {0x22, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x23, 8, &Cpu::RLA, "*RLA", &Cpu::Addr_INX, "INX [XXX]"},
        {0x24, 3, &Cpu::BIT, "BIT", &Cpu::Addr_ZP0, "ZP0"},
        {0x25, 3, &Cpu::AND, "AND", &Cpu::Addr_ZP0, "ZP0"},
        {0x26, 5, &Cpu::ROL, "ROL", &Cpu::Addr_ZP0, "ZP0"},
        {0x27, 5, &Cpu::RLA, "*RLA", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0x28, 4, &Cpu::PLP, "PLP", &Cpu::Addr_IMP, "IMP"},
        {0x29, 2, &Cpu::AND, "AND", &Cpu::Addr_IMM, "IMM"},
        {0x2A, 2, &Cpu::ROL, "ROL", &Cpu::Addr_ACC, "ACC"},
        {0x2B, 2, &Cpu::AAC, "*AAC", &Cpu::Addr_IMM, "IMM"},
        {0x2C, 4, &Cpu::BIT, "BIT", &Cpu::Addr_ABS, "ABS"},
        {0x2D, 4, &Cpu::AND, "AND", &Cpu::Addr_ABS, "ABS"},
        {0x2E, 6, &Cpu::ROL, "ROL", &Cpu::Addr_ABS, "ABS"},
        {0x2F, 6, &Cpu::RLA, "*RLA", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0x30, 2, &Cpu::BMI, "BMI", &Cpu::Addr_REL, "REL"},
        {0x31, 5, &Cpu::AND, "AND", &Cpu::Addr_INY, "INY"},
        {0x32, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x33, 8, &Cpu::RLA, "*RLA", &Cpu::Addr_INY, "INY [XXX]"},
        {0x34, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x35, 4, &Cpu::AND, "AND", &Cpu::Addr_ZPX, "ZPX"},
        {0x36, 6, &Cpu::ROL, "ROL", &Cpu::Addr_ZPX, "ZPX"},
        {0x37, 6, &Cpu::RLA, "*RLA", &Cpu::Addr_ZPX, "ZPX [XXX]"},
        {0x38, 2, &Cpu::SEC, "SEC", &Cpu::Addr_IMP, "IMP"},
        {0x39, 4, &Cpu::AND, "AND", &Cpu::Addr_ABY, "ABY"},
        {0x3A, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x3B, 7, &Cpu::RLA, "*RLA", &Cpu::Addr_ABY, "ABY [XXX]"},
        {0x3C, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x3D, 4, &Cpu::AND, "AND", &Cpu::Addr_ABX, "ABX"},
        {0x3E, 7, &Cpu::ROL, "ROL", &Cpu::Addr_ABX, "ABX"},
        {0x3F, 7, &Cpu::RLA, "*RLA", &Cpu::Addr_ABX, "ABX [XXX]"},

        {0x40, 6, &Cpu::RTI, "RTI", &Cpu::Addr_IMP, "IMP"},
        {0x41, 6, &Cpu::EOR, "EOR", &Cpu::Addr_INX, "INX"},
        {0x42, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x43, 8, &Cpu::SRE, "*SRE", &Cpu::Addr_INX, "INX [XXX]"},
        {0x44, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x45, 3, &Cpu::EOR, "EOR", &Cpu::Addr_ZP0, "ZP0"},
        {0x46, 5, &Cpu::LSR, "LSR", &Cpu::Addr_ZP0, "ZP0"},
        {0x47, 5, &Cpu::SRE, "*SRE", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0x48, 3, &Cpu::PHA, "PHA", &Cpu::Addr_IMP, "IMP"},
        {0x49, 2, &Cpu::EOR, "EOR", &Cpu::Addr_IMM, "IMM"},
        {0x4A, 2, &Cpu::LSR, "LSR", &Cpu::Addr_ACC, "ACC"},
        {0x4B, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x4C, 3, &Cpu::JMP, "JMP", &Cpu::Addr_ABS, "ABS"},
        {0x4D, 4, &Cpu::EOR, "EOR", &Cpu::Addr_ABS, "ABS"},
        {0x4E, 6, &Cpu::LSR, "LSR", &Cpu::Addr_ABS, "ABS"},
        {0x4F, 6, &Cpu::SRE, "*SRE", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0x50, 2, &Cpu::BVC, "BVC", &Cpu::Addr_REL, "REL"},
        {0x51, 5, &Cpu::EOR, "EOR", &Cpu::Addr_INY, "INY"},
        {0x52, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x53, 8, &Cpu::SRE, "*SRE", &Cpu::Addr_INY, "INY [XXX]"},
        {0x54, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x55, 4, &Cpu::EOR, "EOR", &Cpu::Addr_ZPX, "ZPX"},
        {0x56, 6, &Cpu::LSR, "LSR", &Cpu::Addr_ZPX, "ZPX"},
        {0x57, 6, &Cpu::SRE, "*SRE", &Cpu::Addr_ZPX, "ZPX [XXX]"},
        {0x58, 2, &Cpu::CLI, "CLI", &Cpu::Addr_IMP, "IMP"},
        {0x59, 4, &Cpu::EOR, "EOR", &Cpu::Addr_ABY, "ABY"},
        {0x5A, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x5B, 7, &Cpu::SRE, "*SRE", &Cpu::Addr_ABY, "ABY [XXX]"},
        {0x5C, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x5D, 4, &Cpu::EOR, "EOR", &Cpu::Addr_ABX, "ABX"},
        {0x5E, 7, &Cpu::LSR, "LSR", &Cpu::Addr_ABX, "ABX"},
        {0x5F, 7, &Cpu::SRE, "*SRE", &Cpu::Addr_ABX, "ABX [XXX]"},

        {0x60, 6, &Cpu::RTS, "RTS", &Cpu::Addr_IMP, "IMP"},
        {0x61, 6, &Cpu::ADC, "ADC", &Cpu::Addr_INX, "INX"},
        {0x62, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x63, 8, &Cpu::RRA, "*RRA", &Cpu::Addr_INX, "INX [XXX]"},
        {0x64, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x65, 3, &Cpu::ADC, "ADC", &Cpu::Addr_ZP0, "ZP0"},
        {0x66, 5, &Cpu::ROR, "ROR", &Cpu::Addr_ZP0, "ZP0"},
        {0x67, 5, &Cpu::RRA, "*RRA", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0x68, 4, &Cpu::PLA, "PLA", &Cpu::Addr_IMP, "IMP"},
        {0x69, 2, &Cpu::ADC, "ADC", &Cpu::Addr_IMM, "IMM"},
        {0x6A, 2, &Cpu::ROR, "ROR", &Cpu::Addr_ACC, "ACC"},
        {0x6B, 2, &Cpu::AAR, "*AAR", &Cpu::Addr_IMM, "IMM [XXX]"},
        {0x6C, 5, &Cpu::JMP, "JMP", &Cpu::Addr_IND, "IND"},
        {0x6D, 4, &Cpu::ADC, "ADC", &Cpu::Addr_ABS, "ABS"},
        {0x6E, 6, &Cpu::ROR, "ROR", &Cpu::Addr_ABS, "ABS"},
        {0x6F, 6, &Cpu::RRA, "*RRA", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0x70, 2, &Cpu::BVS, "BVS", &Cpu::Addr_REL, "REL"},
        {0x71, 5, &Cpu::ADC, "ADC", &Cpu::Addr_INY, "INY"},
        {0x72, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x73, 8, &Cpu::RRA, "*RRA", &Cpu::Addr_INY, "INY [XXX]"},
        {0x74, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x75, 4, &Cpu::ADC, "ADC", &Cpu::Addr_ZPX, "ZPX"},
        {0x76, 6, &Cpu::ROR, "ROR", &Cpu::Addr_ZPX, "ZPX"},
        {0x77, 6, &Cpu::RRA, "*RRA", &Cpu::Addr_ZPX, "ZPX [XXX]"},
        {0x78, 2, &Cpu::SEI, "SEI", &Cpu::Addr_IMP, "IMP"},
        {0x79, 4, &Cpu::ADC, "ADC", &Cpu::Addr_ABY, "ABY"},
        {0x7A, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x7B, 7, &Cpu::RRA, "*RRA", &Cpu::Addr_ABY, "ABY [XXX]"},
        {0x7C, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x7D, 4, &Cpu::ADC, "ADC", &Cpu::Addr_ABX, "ABX"},
        {0x7E, 7, &Cpu::ROR, "ROR", &Cpu::Addr_ABX, "ABX"},
        {0x7F, 7, &Cpu::RRA, "*RRA", &Cpu::Addr_ABX, "ABX [XXX]"},

        {0x80, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0x81, 6, &Cpu::STA, "STA", &Cpu::Addr_INX, "INX"},
        {0x82, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x83, 6, &Cpu::AAX, "*AAX", &Cpu::Addr_INX, "INX [XXX]"},
        {0x84, 3, &Cpu::STY, "STY", &Cpu::Addr_ZP0, "ZP0"},
        {0x85, 3, &Cpu::STA, "STA", &Cpu::Addr_ZP0, "ZP0"},
        {0x86, 3, &Cpu::STX, "STX", &Cpu::Addr_ZP0, "ZP0"},
        {0x87, 3, &Cpu::AAX, "*AAX", &Cpu::Addr_ZP0, "ZP0"},
        {0x88, 2, &Cpu::DEY, "DEY", &Cpu::Addr_IMP, "IMP"},
        {0x89, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x8A, 2, &Cpu::TXA, "TXA", &Cpu::Addr_IMP, "IMP"},
        {0x8B, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x8C, 4, &Cpu::STY, "STY", &Cpu::Addr_ABS, "ABS"},
        {0x8D, 4, &Cpu::STA, "STA", &Cpu::Addr_ABS, "ABS"},
        {0x8E, 4, &Cpu::STX, "STX", &Cpu::Addr_ABS, "ABS"},
        {0x8F, 4, &Cpu::AAX, "*AAX", &Cpu::Addr_ABS, "INX [XXX]"},

        {0x90, 2, &Cpu::BCC, "BCC", &Cpu::Addr_REL, "REL"},
        {0x91, 6, &Cpu::STA, "STA", &Cpu::Addr_INY, "INY"},
        {0x92, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x93, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x94, 4, &Cpu::STY, "STY", &Cpu::Addr_ZPX, "ZPX"},
        {0x95, 4, &Cpu::STA, "STA", &Cpu::Addr_ZPX, "ZPX"},
        {0x96, 4, &Cpu::STX, "STX", &Cpu::Addr_ZPY, "ZPY"},
        {0x97, 4, &Cpu::AAX, "*AAX", &Cpu::Addr_ZPY, "ZPY"},
        {0x98, 2, &Cpu::TYA, "TYA", &Cpu::Addr_IMP, "IMP"},
        {0x99, 5, &Cpu::STA, "STA", &Cpu::Addr_ABY, "ABY"},
        {0x9A, 2, &Cpu::TXS, "TXS", &Cpu::Addr_IMP, "IMP"},
        {0x9B, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x9C, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x9D, 5, &Cpu::STA, "STA", &Cpu::Addr_ABX, "ABX"},
        {0x9E, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0x9F, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},

        {0xA0, 2, &Cpu::LDY, "LDY", &Cpu::Addr_IMM, "IMM"},
        {0xA1, 6, &Cpu::LDA, "LDA", &Cpu::Addr_INX, "INX"},
        {0xA2, 2, &Cpu::LDX, "LDX", &Cpu::Addr_IMM, "IMM"},
        {0xA3, 0, &Cpu::LAX, "*LAX", &Cpu::Addr_INX, "INX [XXX]"},
        {0xA4, 3, &Cpu::LDY, "LDY", &Cpu::Addr_ZP0, "ZP0"},
        {0xA5, 3, &Cpu::LDA, "LDA", &Cpu::Addr_ZP0, "ZP0"},
        {0xA6, 3, &Cpu::LDX, "LDX", &Cpu::Addr_ZP0, "ZP0"},
        {0xA7, 0, &Cpu::LAX, "*LAX", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0xA8, 2, &Cpu::TAY, "TAY", &Cpu::Addr_IMP, "IMP"},
        {0xA9, 2, &Cpu::LDA, "LDA", &Cpu::Addr_IMM, "IMM"},
        {0xAA, 2, &Cpu::TAX, "TAX", &Cpu::Addr_IMP, "IMP"},
        {0xAB, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xAC, 4, &Cpu::LDY, "LDY", &Cpu::Addr_ABS, "ABS"},
        {0xAD, 4, &Cpu::LDA, "LDA", &Cpu::Addr_ABS, "ABS"},
        {0xAE, 4, &Cpu::LDX, "LDX", &Cpu::Addr_ABS, "ABS"},
        {0xAF, 0, &Cpu::LAX, "*LAX", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0xB0, 2, &Cpu::BCS, "BCS", &Cpu::Addr_REL, "REL"},
        {0xB1, 5, &Cpu::LDA, "LDA", &Cpu::Addr_INY, "INY"},
        {0xB2, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xB3, 0, &Cpu::LAX, "*LAX", &Cpu::Addr_INY, "INY [XXX]"},
        {0xB4, 4, &Cpu::LDY, "LDY", &Cpu::Addr_ZPX, "ZPX"},
        {0xB5, 4, &Cpu::LDA, "LDA", &Cpu::Addr_ZPX, "ZPX"},
        {0xB6, 4, &Cpu::LDX, "LDX", &Cpu::Addr_ZPY, "ZPY"},
        {0xB7, 0, &Cpu::LAX, "*LAX", &Cpu::Addr_ZPY, "ZPY [XXX]"},
        {0xB8, 2, &Cpu::CLV, "CLV", &Cpu::Addr_IMP, "IMP"},
        {0xB9, 4, &Cpu::LDA, "LDA", &Cpu::Addr_ABY, "ABY"},
        {0xBA, 2, &Cpu::TSX, "TSX", &Cpu::Addr_IMP, "IMP"},
        {0xBB, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xBC, 4, &Cpu::LDY, "LDY", &Cpu::Addr_ABX, "APX"},
        {0xBD, 4, &Cpu::LDA, "LDA", &Cpu::Addr_ABX, "APX"},
        {0xBE, 4, &Cpu::LDX, "LDX", &Cpu::Addr_ABY, "ABY"},
        {0xBF, 0, &Cpu::LAX, "*LAX", &Cpu::Addr_ABY, "ABY [XXX]"},

        {0xC0, 2, &Cpu::CPY, "CPY", &Cpu::Addr_IMM, "IMM"},
        {0xC1, 6, &Cpu::CMP, "CMP", &Cpu::Addr_INX, "INX"},
        {0xC2, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xC3, 8, &Cpu::DCP, "*DCP", &Cpu::Addr_INX, "INX [XXX]"},
        {0xC4, 3, &Cpu::CPY, "CPY", &Cpu::Addr_ZP0, "ZP0"},
        {0xC5, 3, &Cpu::CMP, "CMP", &Cpu::Addr_ZP0, "ZP0"},
        {0xC6, 5, &Cpu::DEC, "DEC", &Cpu::Addr_ZP0, "ZP0"},
        {0xC7, 5, &Cpu::DCP, "*DCP", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0xC8, 2, &Cpu::INY, "INY", &Cpu::Addr_IMP, "IMP"},
        {0xC9, 2, &Cpu::CMP, "CMP", &Cpu::Addr_IMM, "IMM"},
        {0xCA, 2, &Cpu::DEX, "DEX", &Cpu::Addr_IMP, "IMP"},
        {0xCB, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xCC, 4, &Cpu::CPY, "CPY", &Cpu::Addr_ABS, "ABS"},
        {0xCD, 4, &Cpu::CMP, "CMP", &Cpu::Addr_ABS, "ABS"},
        {0xCE, 6, &Cpu::DEC, "DEC", &Cpu::Addr_ABS, "ABS"},
        {0xCF, 6, &Cpu::DCP, "*DCP", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0xD0, 2, &Cpu::BNE, "BNE", &Cpu::Addr_REL, "REL"},
        {0xD1, 5, &Cpu::CMP, "CMP", &Cpu::Addr_INY, "INY"},
        {0xD2, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xD3, 8, &Cpu::DCP, "*DCP", &Cpu::Addr_INY, "INY [XXX]"},
        {0xD4, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0xD5, 4, &Cpu::CMP, "CMP", &Cpu::Addr_ZPX, "ZPX"},
        {0xD6, 6, &Cpu::DEC, "DEC", &Cpu::Addr_ZPX, "ZPX"},
        {0xD7, 6, &Cpu::DCP, "*DCP", &Cpu::Addr_ZPX, "ZPX [XXX]"},
        {0xD8, 2, &Cpu::CLD, "CLD", &Cpu::Addr_IMP, "IMP"},
        {0xD9, 4, &Cpu::CMP, "CMP", &Cpu::Addr_ABY, "ABY"},
        {0xDA, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0xDB, 7, &Cpu::DCP, "*DCP", &Cpu::Addr_ABY, "ABY [XXX]"},
        {0xDC, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0xDD, 4, &Cpu::CMP, "CMP", &Cpu::Addr_ABX, "ABX"},
        {0xDE, 7, &Cpu::DEC, "DEC", &Cpu::Addr_ABX, "ABX"},
        {0xDF, 7, &Cpu::DCP, "*DCP", &Cpu::Addr_ABX, "ABX [XXX]"},

        {0xE0, 2, &Cpu::CPX, "CPX", &Cpu::Addr_IMM, "IMM"},
        {0xE1, 6, &Cpu::SBC, "SBC", &Cpu::Addr_INX, "INX"},
        {0xE2, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xE3, 8, &Cpu::ISC, "*ISC", &Cpu::Addr_INX, "INX [XXX]"},
        {0xE4, 3, &Cpu::CPX, "CPX", &Cpu::Addr_ZP0, "ZP0"},
        {0xE5, 3, &Cpu::SBC, "SBC", &Cpu::Addr_ZP0, "ZP0"},
        {0xE6, 5, &Cpu::INC, "INC", &Cpu::Addr_ZP0, "ZP0"},
        {0xE7, 5, &Cpu::ISC, "*ISC", &Cpu::Addr_ZP0, "ZP0 [XXX]"},
        {0xE8, 2, &Cpu::INX, "INX", &Cpu::Addr_IMP, "IMP"},
        {0xE9, 2, &Cpu::SBC, "SBC", &Cpu::Addr_IMM, "IMM"},
        {0xEA, 2, &Cpu::NOP, "NOP", &Cpu::Addr_IMP, "IMP"},
        {0xEB, 2, &Cpu::SBC, "*SBC", &Cpu::Addr_IMM, "IMM"},      // Illegal SBC (Same as normal)
        {0xEC, 4, &Cpu::CPX, "CPX", &Cpu::Addr_ABS, "ABS"},
        {0xED, 4, &Cpu::SBC, "SBC", &Cpu::Addr_ABS, "ABS"},
        {0xEE, 6, &Cpu::INC, "INC", &Cpu::Addr_ABS, "ABS"},
        {0xEF, 6, &Cpu::ISC, "*ISC", &Cpu::Addr_ABS, "ABS [XXX]"},

        {0xF0, 2, &Cpu::BEQ, "BEQ", &Cpu::Addr_REL, "REL"},
        {0xF1, 5, &Cpu::SBC, "SBC", &Cpu::Addr_INY, "INY"},
        {0xF2, 0, &Cpu::XXX, "XXX", &Cpu::XXX, "XXX"},
        {0xF3, 4, &Cpu::ISC, "*ISC", &Cpu::Addr_INY, "INY [XXX]"},
        {0xF4, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0xF5, 4, &Cpu::SBC, "SBC", &Cpu::Addr_ZPX, "ZPX"},
        {0xF6, 6, &Cpu::INC, "INC", &Cpu::Addr_ZPX, "ZPX"},
        {0xF7, 6, &Cpu::ISC, "*ISC", &Cpu::Addr_ZPX, "ZPX [XXX]"},
        {0xF8, 2, &Cpu::SED, "SED", &Cpu::Addr_IMP, "IMP"},
        {0xF9, 4, &Cpu::SBC, "SBC", &Cpu::Addr_ABY, "ABY"},
        {0xFA, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0xFB, 7, &Cpu::ISC, "*ISC", &Cpu::Addr_ABY, "XXX"},
        {0xFC, 0, &Cpu::NOP, "*NOP", &Cpu::XXX, "XXX"},
        {0xFD, 4, &Cpu::SBC, "SBC", &Cpu::Addr_ABX, "ABX"},
        {0xFE, 7, &Cpu::INC, "INC", &Cpu::Addr_ABX, "ABX"},
        {0xFF, 7, &Cpu::ISC, "*ISC", &Cpu::Addr_ABX, "ABX [XXX]"}

    };

    disassembly_line.reserve(300);
}

// Deconstructor
Cpu::~Cpu(){}


// ----------------BUS CONNECTIONS-------------------//
inline uint8_t Cpu::read(uint16_t a) 
{
    return bus->read(a, false);      // Read from Bus
}

// Write to Bus
inline void Cpu::write(uint16_t a, uint8_t d)
{
    bus->write(a, d);                // Write to Bus
}


// -----------------STATUS REGISTER-------------------//
uint8_t Cpu::getStatusRegister(StatusFlags flags)
{
    if ((reg.P & flags) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    };
}

void Cpu::setStatusRegister(StatusFlags flags, bool mode)
{
    if(mode) 
    {
        reg.P = reg.P | flags;    // Set Flags
    } 
    else 
    {
        reg.P = reg.P & ~flags;   // Clear Flags
    };
}


//-------------INTERRUPTS & CPU FUNCTIONS---------------//
void Cpu::reset()
{
    reg.S  = 0xFD;
    reg.A = 0x00;
    reg.X = 0x00;
    reg.Y = 0x00;
    reg.P = 0x00;
    
    con.cycles = 8;                                // Reset takes 8 cycles
    con.mem_addr = 0xFFFC;

    uint16_t low_byte = read(0xFFFC);        // Read from 0xFFFC
    uint16_t high_byte = read(0xFFFD);   // Read from 0xFFFD

    reg.PC = (high_byte << 8) | (low_byte);      // Shift high_byte left 8 bits, add both hi and lo bytes
    //reg.PC = 0xC000;

    reg.P = 0x00 | U;                            // U flag is always set to 1 

    // Initialize to 0
    con.fetched = 0x00;
    con.rel_addr = 0x0000;
    con.mem_addr = 0x0000;

}

void Cpu::irq()
{   // If interrupts are enabled
    if(getStatusRegister(I)) {     

        write(0x0100 + reg.S, (reg.PC >> 8) & 0x00FF);    // Stack starts at location 0x0100 -> 0x01FF
        reg.S--;                                             // Stack is decremented each iteration
        write(0x0100 + reg.S, reg.PC & 0x00FF);
        reg.S--;

        setStatusRegister(B, 0);    // B flag is always 0
        setStatusRegister(U, 1);    // Unused, always 1  
        setStatusRegister(I, 1);  
        write(0x0100 + reg.S, reg.P);
        reg.S--;

        con.mem_addr = 0xFFFE;
        uint16_t low_byte = read(con.mem_addr);        // Read 0xFFFE
        uint16_t high_byte = read(con.mem_addr + 1);   // Read 0xFFFF

        reg.PC = (high_byte << 8) | (low_byte);  
        con.cycles = 7;
    }
}

void Cpu::nmi()
{
    write(0x0100 + reg.S, (reg.PC >> 8) & 0x00FF);    // Stack starts at location 0x0100 -> 0x01FF
    reg.S--;                                      // Stack is decremented each iteration
    write(0x0100 + reg.S, reg.PC & 0x00FF);
    reg.S--;

    setStatusRegister(B, 0);    // B flag is always 0
    setStatusRegister(U, 1);    // Unused, always 1
    setStatusRegister(I, 1);
    write(0x0100 + reg.S, reg.P);
    reg.S--;

    con.mem_addr = 0xFFFA;
    uint16_t low_byte = read(con.mem_addr);        // Read 0xFFFA
    uint16_t high_byte = read(con.mem_addr + 1);   // Read 0xFFFB
    reg.PC = (high_byte << 8) | low_byte;

    con.cycles = 8;
}


// ---------------CPU executor & fetcher--------------------//
uint16_t Cpu::tick()
{
    newInstruction = false;
    if(con.cycles == 0)
    {
        newInstruction = true;
        // Read instruction from PC to perform next instruction
        con.opcode = read(reg.PC);

        if(renderDisassembly)
        {
            d.get_instruction.clear();
            d.get_addrmode.clear();
            d.mem_location.clear();

            // Log Instruction Memory Location
            ss.str( std::string() );
            ss.clear();

            ss << std::hex << (int) reg.PC;
            d.mem_location = "$" + ss.str();
        };

        // Increment PC
        reg.PC++;   
        setStatusRegister(U, true);

        con.cycles = op_lookup[con.opcode].op_cycles;                                   // Get # of Cycles

        // Log operations
        d.get_instruction = op_lookup[con.opcode].op_sInstruction;
        d.get_addrmode = op_lookup[con.opcode].addrMode;
        d.get_opcode = op_lookup[con.opcode].op_opcode;

        // Complete Instruction operation
        uint8_t address_mode = (this->*op_lookup[con.opcode].op_addressmode)();     // Get CPU Address Mode 
        uint8_t instruction = (this->*op_lookup[con.opcode].op_instruction)();      // Get CPU Instruction to perform

        // Add potential, additional cycles
        con.cycles += (address_mode & instruction);     
        setStatusRegister(U, true); 

    };
    if(con.cycles > 0)
    {
        con.cycles--;
    };
    return reg.PC;
}


inline void Cpu::setDisassemblyLog(std::string &disassem)
{
    //disassem = std::to_string(disCount) + ". " + disassem;
    if(renderDisassembly)
    {
        d.disassembly.push_front(disassem);
        max_size = 18;

        if(disassemblySize > max_size)
        {
            d.disassembly.pop_back();
        }
        else
        {
            disassemblySize++;
        };
        disCount++;
        thisDis = disassem;
    };

}

inline void Cpu::setDisassemblyOp(uint8_t op1, uint8_t op2, bool mult_ops)
{
    if(renderDisassembly)
    {
        ss1.str( std::string() );
        ss1.clear();

        ss2.str( std::string() );
        ss2.clear();

        ss1 << std::hex << (int) op1;
        ss2 << std::hex << (int) op2;

        if(op1 == 0)
        {
            d.get_opcode_instruction[0] == "  ";
        };

        d.get_opcode_instruction[0] = ss1.str();

        if(mult_ops == true)
        {
            d.get_opcode_instruction[1] = ss2.str();
        }
        else
        {
            d.get_opcode_instruction[1] = "  ";
        };
    };
}

uint8_t Cpu::transferDMA(uint8_t &addr_offset, uint8_t &dma_addr)
{   
    uint8_t data = 0x00;
    data = read(addr_offset << 8 | dma_addr);
    return data;
}


inline uint8_t Cpu::fetchInstruction()
{   
    // If implied Instruction
    if(!(op_lookup[con.opcode].op_addressmode == &Cpu::Addr_IMP) && !(op_lookup[con.opcode].op_addressmode == &Cpu::Addr_ACC)) 
    {
        con.fetched = read(con.mem_addr);
    };
    return con.fetched;
}


//---------------CPU Addressing Modes------------------------//
// Implicit / Implied
uint8_t Cpu::Addr_IMP()
{
    con.fetched = reg.A;

    if(renderDisassembly)
    {
        disassembly_line = d.mem_location + ": " + d.get_instruction + " [" + d.get_addrmode + "]";
        setDisassemblyOp(0, 0, false);
        setDisassemblyLog(disassembly_line);
    };
    return 0;
}
// Accumulator
uint8_t Cpu::Addr_ACC()
{
    con.fetched = reg.A;
    if(renderDisassembly)
    {
        setDisassemblyOp(con.fetched, 0, false);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return 0;
}
// Immediate
uint8_t Cpu::Addr_IMM()
{   
    con.mem_addr = reg.PC++;
    if(renderDisassembly)
    {
        setDisassemblyOp(read(con.mem_addr), 0, false);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " #$" + d.get_opcode_instruction[0] + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return 0;
}
// Zero Page
uint8_t Cpu::Addr_ZP0()
{
    con.mem_addr = read(reg.PC);
    reg.PC++;

    con.mem_addr &= 0x00FF;

    if(renderDisassembly)
    {
        setDisassemblyOp( (uint8_t)(con.mem_addr & 0x00FF), 0, false);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[0] + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return 0;
}
// Zero Page X
uint8_t Cpu::Addr_ZPX()
{
    con.mem_addr = (read(reg.PC) + reg.X);
    reg.PC++;

    con.mem_addr &= 0x00FF;

    if(renderDisassembly)
    {
        setDisassemblyOp( (uint8_t)(con.mem_addr & 0x00FF), 0, false);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[0] + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return 0;
} 
// Zero Page Y 
uint8_t Cpu::Addr_ZPY()
{
    con.mem_addr = (read(reg.PC) + reg.Y);
    reg.PC++;

    con.mem_addr &= 0x00FF;

    if(renderDisassembly)
    {
        setDisassemblyOp( (uint8_t)(con.mem_addr & 0x00FF), 0, false);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[0] + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };
    return 0;
}
// Relative
uint8_t Cpu::Addr_REL()
{
    con.rel_addr = read(reg.PC);
    reg.PC++;

    if (con.rel_addr & 0x80)
    {
        con.rel_addr |= 0xFF00;
    };

    if(renderDisassembly)
    {
        uint16_t addr = con.rel_addr + reg.PC;
        uint8_t lo = (addr & 0x00FF);
        uint8_t hi = ((addr >> 8) & 0x00FF);

        ss1.str( std::string() );
        ss1.clear();
        ss1 << std::hex << (int) hi; 
        ss1 << std::hex << (int) lo;

        setDisassemblyOp( (uint8_t)(con.rel_addr), 0, false);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + ss.str() + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return 0;
}
// Absolute
uint8_t Cpu::Addr_ABS()
{
    uint16_t low_byte = read(reg.PC);
    reg.PC++;

    uint16_t high_byte = read(reg.PC);
    reg.PC++;

    con.mem_addr = (high_byte << 8) | low_byte;

    if(renderDisassembly)
    {
        uint8_t lo = (uint8_t)((low_byte & 0x00FF));
        uint8_t hi = (uint8_t)(high_byte & 0x00FF);

        setDisassemblyOp(lo, hi, true);
        disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[1] + d.get_opcode_instruction[0] + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return 0;
}
// Absolute X
uint8_t Cpu::Addr_ABX()
{
    uint16_t low_byte = read(reg.PC);
    reg.PC++;

    uint16_t high_byte = read(reg.PC);
    reg.PC++;

    con.mem_addr = (high_byte << 8) | low_byte;
    con.mem_addr += reg.X;

    uint8_t lo =  (uint8_t)((low_byte & 0x00FF));
    uint8_t hi =  (uint8_t)(high_byte & 0x00FF);

    setDisassemblyOp(lo, hi, true);
    disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[1] + d.get_opcode_instruction[0] + ",X [" + d.get_addrmode + "]";
    setDisassemblyLog(disassembly_line);

    if ((con.mem_addr & 0xFF00) != (high_byte << 8))
    {
		return 1;
    }
	else
    {
		return 0;
    };

    return 0;
}
// Absolute Y
uint8_t Cpu::Addr_ABY()
{
    uint16_t low_byte = read(reg.PC);
    reg.PC++;

    uint16_t high_byte = read(reg.PC);
    reg.PC++;

    con.mem_addr = (high_byte << 8) | low_byte;
    con.mem_addr += reg.Y;

    uint8_t lo =  (uint8_t)((low_byte & 0x00FF));
    uint8_t hi =  (uint8_t)(high_byte & 0x00FF);

    setDisassemblyOp(lo, hi, true);
    disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[1] + d.get_opcode_instruction[0] + ",Y [" + d.get_addrmode + "]";
    setDisassemblyLog(disassembly_line);

    if ((con.mem_addr & 0xFF00) != (high_byte << 8))
    {
		return 1;
    }
	else
    {
		return 0;
    };
    
    return 0;
}
// Inderect
uint8_t Cpu::Addr_IND()
{
    uint16_t low_byte = read(reg.PC);
    reg.PC++;

    uint16_t high_byte = read(reg.PC);
    reg.PC++;

    uint16_t pointer = (high_byte << 8) | low_byte;

    if (low_byte == 0x00FF) // Simulate page boundary hardware bug
	{
		con.mem_addr = (read(pointer & 0xFF00) << 8) | read(pointer + 0);
	}
	else // Behave normally
	{
		con.mem_addr = (read(pointer + 1) << 8) | read(pointer + 0);
	};
    

    uint8_t lo = (uint8_t)((low_byte & 0x00FF));
    uint8_t hi = (uint8_t)(high_byte & 0x00FF);

    setDisassemblyOp(lo, hi, true);
    disassembly_line = d.mem_location + ": " + d.get_instruction + " ($" + d.get_opcode_instruction[1] + d.get_opcode_instruction[0] + ") [" + d.get_addrmode + "]";
    setDisassemblyLog(disassembly_line);

    return 0;
}
// Indirect Indexed X
uint8_t Cpu::Addr_INX()
{
    uint16_t zpl = read(reg.PC);
    reg.PC++;

    uint16_t low_byte = read((uint16_t)(zpl + (uint16_t)reg.X) & 0x00FF);
	uint16_t high_byte = read((uint16_t)(zpl + (uint16_t)reg.X + 1) & 0x00FF);

    con.mem_addr = (high_byte << 8) | low_byte;

    setDisassemblyOp(zpl, 0, false);
    disassembly_line = d.mem_location + ": " + d.get_instruction + " ($" + d.get_opcode_instruction[0] + ",X) [" + d.get_addrmode + "]";
    setDisassemblyLog(disassembly_line);

    return 0;
}
// Indirect Indexed Y
uint8_t Cpu::Addr_INY()
{
    uint16_t zpl = read(reg.PC);
	reg.PC++;

	uint16_t lo = read(zpl & 0x00FF);
	uint16_t hi = read((zpl + 1) & 0x00FF);

	con.mem_addr = (hi << 8) | lo;
    con.mem_addr += reg.Y;

    setDisassemblyOp(zpl, 0, false);
    disassembly_line = d.mem_location + ": " + d.get_instruction + " ($" + d.get_opcode_instruction[0] + "),Y [" + d.get_addrmode + "]";
    setDisassemblyLog(disassembly_line);
	
	if ((con.mem_addr & 0xFF00) != (hi << 8))
    {
		return 1;
    }
	else
    {
		return 0;
    };
}



//---------------CPU Instructions------------------------//
uint8_t Cpu::ADC()
{
    
    fetchInstruction();
    con.temp = (uint16_t)reg.A + (uint16_t)con.fetched + (uint16_t)getStatusRegister(C);
    //con.temp = con.temp - 0x01;

    setStatusRegister(C, (con.temp > 0x00FF));
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(V, (~((uint16_t)reg.A ^ (uint16_t)con.fetched) & ((uint16_t)reg.A ^ (uint16_t)con.temp)) & 0x0080);
    setStatusRegister(N, con.temp & N);

    reg.A = con.temp & 0x00FF;
    return 1;
}

uint8_t Cpu::AND()
{
    fetchInstruction();
    reg.A = reg.A & con.fetched;

    setStatusRegister(Z, (reg.A == 0x00));
    setStatusRegister(N, (reg.A & N));

    return 1;
}

uint8_t Cpu::ASL()
{
    fetchInstruction();
    con.temp = (uint16_t) (con.fetched << 1);

    setStatusRegister(C, (con.temp & 0xFF00) > 0);
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x00);
    setStatusRegister(N, (con.temp & N));
    if (op_lookup[con.opcode].op_addressmode == &Cpu::Addr_ACC /*|| op_lookup[con.opcode].op_addressmode == &Cpu::Addr_IMP*/)
    {
        reg.A = con.temp & 0x00FF;
    }
    else
    {
        write(con.mem_addr, (con.temp & 0x00FF));
    };
    return 0;
}

uint8_t Cpu::BCC()
{
    if(getStatusRegister(C) == 0)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        }
        reg.PC = con.mem_addr;
        
    }
    return 0;
}

uint8_t Cpu::BCS()
{
    if(getStatusRegister(C) == 1)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        };
        reg.PC = con.mem_addr;
    };
    return 0;
}

uint8_t Cpu::BEQ()
{
    if(getStatusRegister(Z) == 1)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        };
        reg.PC = con.mem_addr;
    };
    return 0;
}

uint8_t Cpu::BIT()
{
    fetchInstruction();
    con.temp = con.fetched & reg.A;
    setStatusRegister(Z, (con.temp & 0x00FF) == 0);
    setStatusRegister(V, con.fetched & V);
    setStatusRegister(N, con.fetched & N);

    return 0;
}

uint8_t Cpu::BMI()
{
    if(getStatusRegister(N) == 1)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        };
        reg.PC = con.mem_addr;
    };
    return 0;
}

uint8_t Cpu::BNE()
{
    if(getStatusRegister(Z) == 0)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        };
        reg.PC = con.mem_addr;
    };

    return 0;
}

uint8_t Cpu::BPL()
{
    if(getStatusRegister(N) == 0)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        }
        reg.PC = con.mem_addr;
    }
    return 0;
}

uint8_t Cpu::BRK()
{
    reg.PC++;
	
	setStatusRegister(I, true);
	write(0x0100 + reg.S, (reg.PC >> 8) & 0x00FF);
	reg.S--;

	write(0x0100 + reg.S, reg.PC & 0x00FF);
	reg.S--;

	setStatusRegister(B, true);
	write(0x0100 + reg.S, reg.P);
	reg.S--;

	setStatusRegister(B, false);

	reg.PC = (uint16_t) read(0xFFFE) | ((uint16_t) read(0xFFFF) << 8);
	return 0;
}

uint8_t Cpu::BVC()
{
     if(getStatusRegister(V) == 0)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        }
        reg.PC = con.mem_addr;
    }
    return 0;
}

uint8_t Cpu::BVS()
{
     if(getStatusRegister(V) == 1)
    {
        con.cycles++;
        con.mem_addr = reg.PC + con.rel_addr;

        if((con.mem_addr & 0xFF00) != (reg.PC & 0xFF00))
        {
            con.cycles++;
        }
        reg.PC = con.mem_addr;
    }
    return 0;
}

uint8_t Cpu::CLC()
{
    setStatusRegister(C, false);
    return 0;
}

uint8_t Cpu::CLD()
{
    setStatusRegister(D, false);
    return 0;
}

uint8_t Cpu::CLI()
{
    setStatusRegister(I, false);
    return 0;
}

uint8_t Cpu::CLV()
{
    setStatusRegister(V, false);
    return 0;
}

uint8_t Cpu::CMP()
{
    fetchInstruction();
    con.temp = (uint16_t)reg.A - (uint16_t)con.fetched;

    setStatusRegister(C, (reg.A >= con.fetched));                
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, (con.temp & N));
    return 1;
}

uint8_t Cpu::CPX()
{
    fetchInstruction();
    con.temp = (uint16_t)reg.X - (uint16_t)con.fetched;

    setStatusRegister(C, (reg.X >= con.fetched));                  
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, (con.temp & N));
    return 0;
}

uint8_t Cpu::CPY()
{
    fetchInstruction();
    con.temp = (uint16_t)reg.Y - (uint16_t)con.fetched;

    setStatusRegister(C, (reg.Y >= con.fetched));                   
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, (con.temp & N));
    return 0;
}

uint8_t Cpu::DEC()
{
    fetchInstruction();
    con.temp = con.fetched - 1;
    write(con.mem_addr, con.temp & 0x00FF);

    setStatusRegister(Z, (con.temp & 0x00FF) == 0x00);
    setStatusRegister(N, (con.temp & N));

    return 0;
}

uint8_t Cpu::DEX()
{
    reg.X--;
    setStatusRegister(Z, reg.X == 0x00);
    setStatusRegister(N, (reg.X & N));

    return 0;
}

uint8_t Cpu::DEY()
{
    reg.Y--;
    setStatusRegister(Z, reg.Y == 0x00);
    setStatusRegister(N, (reg.Y & N));

    return 0;
}

uint8_t Cpu::EOR()
{
    fetchInstruction();
    reg.A = reg.A ^ con.fetched;
    setStatusRegister(Z, (reg.A == 0x00));
    setStatusRegister(N, (reg.A & N));

    return 1;
}

uint8_t Cpu::INC()
{
    fetchInstruction();
    con.temp = con.fetched + 1;
    write(con.mem_addr, con.temp & 0x00FF);

    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, (con.temp & N));

    return 0;
}

uint8_t Cpu::INX()
{
    reg.X++;
    setStatusRegister(Z, (reg.X == 0x00));
    setStatusRegister(N, (reg.X & N));

    return 0;
}

uint8_t Cpu::INY()
{
    reg.Y++;
    setStatusRegister(Z, (reg.Y == 0x00));
    setStatusRegister(N, (reg.Y & N));

    return 0;
}

uint8_t Cpu::JMP()
{
    reg.PC = con.mem_addr;
    return 0;
}

uint8_t Cpu::JSR()
{
    reg.PC--;

    // Write Low byte to the Stack Address
	write(0x0100 + reg.S, (reg.PC >> 8) & 0x00FF);
	reg.S--;

    // Write High byte to the Stack Address
	write(0x0100 + reg.S, reg.PC & 0x00FF);
	reg.S--;

    // Set PC to Addr
	reg.PC = con.mem_addr;
	return 0;
}

uint8_t Cpu::LDA()
{
    fetchInstruction();
    reg.A = con.fetched;

    setStatusRegister(Z, reg.A == 0x00);
    setStatusRegister(N, (reg.A & N));

    return 1;
}

uint8_t Cpu::LDX()
{
    fetchInstruction();
    reg.X = con.fetched;

    setStatusRegister(Z, reg.X == 0x00);
    setStatusRegister(N, (reg.X & N));

    return 1;
}

uint8_t Cpu::LDY()
{
    fetchInstruction();
    reg.Y = con.fetched;

    setStatusRegister(Z, reg.Y == 0x00);
    setStatusRegister(N, (reg.Y & N));

    return 1;
}

uint8_t Cpu::LSR()
{
    fetchInstruction();
    setStatusRegister(C, (con.fetched & 0x0001));

    con.fetched = con.fetched >> 1;
    con.temp = (uint16_t) con.fetched & 0x00FF;

    setStatusRegister(Z, (con.temp & 0x00FF) == 0x00);
    setStatusRegister(N, (con.temp & N));

    if(!(op_lookup[con.opcode].op_addressmode == &Cpu::Addr_ACC) && (!(op_lookup[con.opcode].op_addressmode == &Cpu::Addr_IMP)))
    {
        write(con.mem_addr, (con.temp & 0x00FF));
        return 0;
    }
    reg.A = con.fetched;
    return 0;
}

uint8_t Cpu::NOP() 
{
    return 0;
}

uint8_t Cpu::ORA() 
{
    fetchInstruction();
    reg.A = reg.A | con.fetched;

    setStatusRegister(Z, reg.A == 0x00);
    setStatusRegister(N, reg.A & N);
    return 1;
}

uint8_t Cpu::PHA()
{
    write(0x0100 + reg.S, reg.A);
	reg.S--;
    return 0;
}

uint8_t Cpu::PHP()
{
    write(0x0100 + reg.S, reg.P | B | U);
    setStatusRegister(B, false);
    setStatusRegister(U, false);
    reg.S--;
    return 0;
}

uint8_t Cpu::PLA() 
{
    reg.S++;
    reg.A = read(0x0100 + reg.S);

    setStatusRegister(Z, reg.A == 0x00);
    setStatusRegister(N, reg.A & N);

    return 0;
}

uint8_t Cpu::PLP()
{
	reg.S++;
	reg.P = read(0x0100 + reg.S);
	setStatusRegister(U, true);
    setStatusRegister(B, false);

	return 0;
}

uint8_t Cpu::ROL()
{
    fetchInstruction();
    con.temp = (uint16_t) (con.fetched << 1) | getStatusRegister(C);
    setStatusRegister(C, con.temp & 0xFF00);
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, con.temp & 0x0080);

    if(op_lookup[con.opcode].op_addressmode == &Cpu::Addr_ACC || op_lookup[con.opcode].op_addressmode == &Cpu::Addr_IMP)
    {
        reg.A = con.temp;
    }
    else
    {
        write(con.mem_addr, con.temp & 0x00FF);
    }
    return 0;
}

uint8_t Cpu::ROR()
{
    fetchInstruction();
    con.temp = (uint16_t) (getStatusRegister(C) << 7) | (con.fetched >> 1 );
    setStatusRegister(C, (con.fetched & C));
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x00);
    setStatusRegister(N, (con.temp & 0x0080));

    if((op_lookup[con.opcode].op_addressmode == &Cpu::Addr_ACC) || (op_lookup[con.opcode].op_addressmode == &Cpu::Addr_IMP))
    {
        reg.A = con.temp & 0x00FF;
    }
    else
    {
        write(con.mem_addr, con.temp & 0x00FF);
    }
    return 0;
}

uint8_t Cpu::RTI()
{
    reg.S++;
    reg.P = read(0x0100 + reg.S);

    reg.P &= ~B;
    reg.P &= ~U;

    reg.S++;
    reg.PC = (uint16_t)read(0x0100 + reg.S);

    reg.S++;
    reg.PC |= (uint16_t)read(0x0100 + reg.S) << 8;

    return 0;
}

uint8_t Cpu::RTS()
{
    reg.S++;
    reg.PC = (uint16_t)read(0x0100 + reg.S);

    reg.S++;
    reg.PC |= (uint16_t)read(0x0100 + reg.S) << 8;

    reg.PC++;
    return 0;
}

uint8_t Cpu::SBC()
{
    fetchInstruction();
	uint16_t fvalue = ((uint16_t)con.fetched) ^ 0x00FF;
	
	con.temp = (uint16_t)reg.A + fvalue + (uint16_t)getStatusRegister(C);

	setStatusRegister(C, con.temp & 0xFF00);
	setStatusRegister(Z, ((con.temp & 0x00FF) == 0));
	setStatusRegister(V, (con.temp ^ (uint16_t)reg.A) & (con.temp ^ fvalue) & 0x0080);
	setStatusRegister(N, (con.temp & N));

	reg.A = con.temp & 0x00FF;
	return 1;
}

uint8_t Cpu::SEC()
{
    setStatusRegister(C, true);
    return 0;
}

uint8_t Cpu::SED()
{
    setStatusRegister(D, true);
    return 0;
}

uint8_t Cpu::SEI()
{
    setStatusRegister(I, true);
    return 0;
}

uint8_t Cpu::STA()
{
    write(con.mem_addr, reg.A);

    return 0;
}

uint8_t Cpu::STX()
{
    write(con.mem_addr, reg.X);

    return 0;
}

uint8_t Cpu::STY()
{
    write(con.mem_addr, reg.Y);

    return 0;
}

uint8_t Cpu::TAX()
{
    reg.X = reg.A;
    setStatusRegister(Z, reg.X == 0x00);
    setStatusRegister(N, reg.X & N);
    return 0;
}

uint8_t Cpu::TAY()
{
    reg.Y = reg.A;
    setStatusRegister(Z, reg.Y == 0x00);
    setStatusRegister(N, reg.Y & N);
    return 0;
}

uint8_t Cpu::TSX()
{
    reg.X = reg.S;
    setStatusRegister(Z, reg.X == 0x00);
    setStatusRegister(N, reg.X & N);
    return 0;
}

uint8_t Cpu::TXA()
{
    reg.A = reg.X;
    setStatusRegister(Z, reg.A == 0x00);
    setStatusRegister(N, reg.A & N);
    return 0;
}

uint8_t Cpu::TXS()
{
    reg.S = reg.X;
    return 0;
}

uint8_t Cpu::TYA()
{
    reg.A = reg.Y;
    setStatusRegister(Z, reg.A == 0x00);
    setStatusRegister(N, reg.A & N);
    return 0;
}


// --------------- ILLEGAL OPCODE INSTRUCTIONS -------------------
uint8_t Cpu::AAC()
{
    fetchInstruction();
    con.fetched &= reg.A;

    setStatusRegister(C, (con.fetched < 0));
    setStatusRegister(Z, (con.fetched == 0x00));
    setStatusRegister(N, (con.fetched & N));

    return 0;
}

uint8_t Cpu::AAX()
{
    uint8_t result = reg.X & reg.A;
    write(con.mem_addr, result);

}

uint8_t Cpu::AAR()
{
    fetchInstruction();
    con.fetched &= reg.A;
    reg.A >> 1;

    uint8_t bit_five = (reg.A & 0x20) > 0;
    uint8_t bit_six = (reg.A & 0x40) > 0;

    // If both bits are 1: set C, clear V
    if ((bit_five & bit_six) > 0x00)
    {
        setStatusRegister(C, true);
    };

    // If both bits are 0: clear C and V
    if ((bit_five | bit_six) == 0x00)
    {
        setStatusRegister(C, false);
        setStatusRegister(V, false);
    };

    // If only bit 5 is 1: set V, clear C
    if ((bit_five == 0x01) && (bit_six == 0x00))
    {
        setStatusRegister(V, true);
        setStatusRegister(C, false);
    };

    // If only bit 6 is 1: set C and V
    if ((bit_five == 0x00) && (bit_six == 0x01))
    {
        setStatusRegister(C, true);
        setStatusRegister(V, true);
    };

    setStatusRegister(Z, (con.fetched == 0x00));
    setStatusRegister(N, ((con.fetched & 0x80) == 0x01));

}

uint8_t Cpu::DCP()
{   

    fetchInstruction();
    con.fetched = con.fetched - 1;
    con.temp = (uint16_t)reg.A - (uint16_t)con.fetched;
    write(con.mem_addr, con.fetched);

    setStatusRegister(C, (reg.A >= con.temp));                  
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, (con.temp & N));
}

uint8_t Cpu::ISC()
{
    fetchInstruction();
    con.fetched += 1;
    write(con.mem_addr, con.fetched);

    uint16_t value = ((uint16_t)con.fetched) ^ 0x00FF;
	
	// Notice this is exactly the same as addition from here!
	con.temp = (uint16_t)reg.A + value + (uint16_t)getStatusRegister(C);
	setStatusRegister(C, con.temp & 0xFF00);
	setStatusRegister(Z, ((con.temp & 0x00FF) == 0));
	setStatusRegister(V, (con.temp ^ (uint16_t)reg.A) & (con.temp ^ value) & 0x0080);
	setStatusRegister(N, con.temp & 0x0080);
	reg.A = con.temp & 0x00FF;
	return 1;
}

uint8_t Cpu::LAX()
{
    fetchInstruction();
    reg.A = con.fetched;
    reg.X = con.fetched;

    setStatusRegister(Z, (reg.X == 0x00));
    setStatusRegister(N, (reg.X & N));

    return 6;   
}

uint8_t Cpu::RLA()
{
    fetchInstruction();
    con.temp = (uint16_t) (con.fetched << 1) | (uint16_t) getStatusRegister(C);
    reg.A &= (con.temp & 0x00FF);

    write(con.mem_addr, con.temp & 0x00FF);

    //setStatusRegister(Z, (reg.A == 0x00));
    //setStatusRegister(N, (reg.A & N));

    setStatusRegister(C, con.temp & 0xFF00);
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(N, con.temp & 0x0080);

    return 1;
}

uint8_t Cpu::RRA()
{
    // ROR
    fetchInstruction();
    uint16_t value = (uint16_t) (getStatusRegister(C) << 7) | (con.fetched >> 1 );
    setStatusRegister(C, (con.fetched & C));


    write(con.mem_addr, value & 0x00FF);


    // ADC
    con.temp = (uint16_t)reg.A + value + (uint16_t)getStatusRegister(C);

    setStatusRegister(C, (con.temp > 0x00FF));
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x0000);
    setStatusRegister(V, (~((uint16_t)reg.A ^ value) & ((uint16_t)reg.A ^ (uint16_t)con.temp)) & 0x0080);
    setStatusRegister(N, con.temp & N);

    reg.A = (con.temp & 0x00FF);
    return 1;
}

uint8_t Cpu::SLO()
{
    fetchInstruction();
    con.temp = (uint16_t)con.fetched << 1;
    write(con.mem_addr, (con.temp & 0x00FF));

    reg.A |= con.temp;

    setStatusRegister(C, (con.temp & 0xFF00) > 0);
    setStatusRegister(Z, (con.temp & 0x00FF) == 0x00);
    setStatusRegister(N, (reg.A & N));

}

uint8_t Cpu::SRE()
{
    fetchInstruction();
    setStatusRegister(C, (con.fetched & 0x01));
    con.temp = (uint16_t)con.fetched >> 1;
    write(con.mem_addr, (con.temp & 0x00FF));

    reg.A ^= (con.temp & 0x00FF);

    //setStatusRegister(C, (con.temp & 0xFF00));
    setStatusRegister(Z, (reg.A == 0x00));
    setStatusRegister(N, (reg.A & N));


}



// Check for Illegal Opcode  (*NOP)
uint8_t Cpu::XXX()
{
    auto illegal_instruct = d.get_opcode;
    bool isIll = false;
    uint8_t add_cycles = 0;

    switch (illegal_instruct)
    {
    case (0x04):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x0C):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;

    case (0x14):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x1A):
        setDisassemblyOp(NULL, NULL, false);
        break;

    case (0x1C):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;
    
    case (0x34):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x3A):
        setDisassemblyOp(NULL, NULL, false);
        break;

    case (0x3C):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;

    case (0x44):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x54):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x5A):
        setDisassemblyOp(NULL, NULL, false);
        break;

    case (0x5C):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;

    case (0x64):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x74):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0x7A):
        setDisassemblyOp(NULL, NULL, false);
        break;

    case (0x7C):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;

    case (0x80):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    // *LAX
    

    case (0xD4):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;

    case (0xDA):
        setDisassemblyOp(NULL, NULL, false);
        break;

    case (0xDC):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;
    
    case (0xF4):
        setDisassemblyOp((read(reg.PC) & 0x00FF), 0, false);
        reg.PC++;
        break;
    
    case (0xFA):
        setDisassemblyOp(NULL, NULL, false);
        break;

    case (0xFC):
        setDisassemblyOp(read(reg.PC), read(reg.PC + 1), true);
        reg.PC = reg.PC + 2;
        break;

    default:
        isIll = false;
        break;
    }


    if(!isIll)
    {
        disassembly_line = d.mem_location + ": " + d.get_instruction + " $" + d.get_opcode_instruction[1] + d.get_opcode_instruction[0] + " [" + d.get_addrmode + "]";
        setDisassemblyLog(disassembly_line);
    };

    return add_cycles;
}