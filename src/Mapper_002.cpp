#include "Mapper_002.h"


Mapper002::Mapper002(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{

}

Mapper002::~Mapper002()
{

}

void Mapper002::reset()
{
    select.addrLo = 0;
    select.addrHi = nPRGBanks - 1;
}

bool Mapper002::mappedPrgRead(uint16_t addr, uint32_t &mappedAddr)
{
    if (addr < 0x8000)
    {
        return false;
    }
    else
    {
        switch(addr)
        {   // Low Address ROM bank
            case 0x8000 ... 0xBFFF:
                mappedAddr = select.addrLo * 16384 + (addr & 0x3FFF);
                break;

            // High Address ROM bank
            case 0xC000 ... 0xFFFF:
                mappedAddr = select.addrHi * 16384 + (addr & 0x3FFF);
                break;
        }
        return true;
    }
}

bool Mapper002::mappedPrgWrite(uint16_t addr, uint32_t &mappedAddr, uint8_t data)
{
    if (addr < 0x8000)
    {
        return false;
    }
    else
    {
        select.addrLo = data & 0x0F;
        return false;
    }
}

bool Mapper002::mappedChrRead(uint16_t addr, uint32_t &mappedAddr)
{
    if(addr < 0x2000)
    {   
        mappedAddr = addr;
        return true;
    };
    return false;
}

bool Mapper002::mappedChrWrite(uint16_t addr, uint32_t &mappedAddr)
{
    if(addr < 0x2000 && nCHRBanks == 0)
    {   
        mappedAddr = addr;
        return true;
    };
    return false;
}