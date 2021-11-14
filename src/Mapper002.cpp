#include "Mapper002.h"


Mapper002::Mapper002(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{

}

Mapper002::~Mapper002()
{

}

void Mapper002::reset()
{
    cpuBank.A = 0;
    cpuBank.B = nPRGBanks - 1;
}

bool Mapper002::mappedPrgRead(uint16_t addr, uint32_t &mappedAddr)
{
    switch(addr)
    {   
        // Unmapped Address Range
        case 0x0000 ... 0x7FFF:
            break;

        // Low Address ROM bank - Bank A
        case 0x8000 ... 0xBFFF:
            mappedAddr = cpuBank.A * PRG_ROM_WINDOW + (addr & PRG_ROM_BANK_SIZE);
            return true;

        // High Address ROM bank - Bank B
        case 0xC000 ... 0xFFFF:
            mappedAddr = cpuBank.B * PRG_ROM_WINDOW + (addr & PRG_ROM_BANK_SIZE);
            return true;
    }
    return false;
}

bool Mapper002::mappedPrgWrite(uint16_t addr, uint32_t &mappedAddr, uint8_t data)
{
    switch(addr)
    {
        // Unmapped Address Range
        case 0x0000 ... 0x7FFF:
            break;

        // Only Low Address ROM bank is Switchable - Bank A
        case 0x8000 ... 0xFFFF:
            cpuBank.A = data & 0x0F;
            break;
    }
    return false;

}

bool Mapper002::mappedChrRead(uint16_t addr, uint32_t &mappedAddr)
{
    switch(addr)
    {
        // Mapped to Pattern Tables in Cartridge
        case 0x0000 ... CHR_ROM_BANK_SIZE:
            mappedAddr = addr;
            return true;
        
        // Unmapped Address Range
        case 0x2000 ... 0xFFFF:
            break;
    }
    return false;
}

bool Mapper002::mappedChrWrite(uint16_t addr, uint32_t &mappedAddr)
{
    switch(addr)
    {
        // Mapped to Pattern Tables in Cartridge
        case 0x0000 ... CHR_ROM_BANK_SIZE:
            if (nCHRBanks == 0)
            {
                mappedAddr = addr;
                return true;
            }
            break;
        
        // Unmapped Address Range
        case 0x2000 ... 0xFFFF:
            break;
    }
    return false;
}