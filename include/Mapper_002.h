#pragma once
#include "Mapper.h"

class Mapper002 : public Mapper
{
    public:
        Mapper002(uint8_t PRG_size, uint8_t CHR_size);
        ~Mapper002();
    
    public:
        // Communication with Bus
        bool mappedPrgRead(uint16_t addr, uint32_t &mappedAddr) override;
        bool mappedPrgWrite(uint16_t addr,  uint32_t &mappedAddr, uint8_t data = 0) override;

        // Communication with PPU Bus
        bool mappedChrRead(uint16_t addr, uint32_t &mappedAddr) override;
        bool mappedChrWrite(uint16_t addr,  uint32_t &mappedAddr) override;

        void reset() override;

        // Bank select ($8000-$FFFF) - Selects 16 KB PRG ROM bank for CPU
        struct BankSelect
        {
            uint8_t addrLo = 0x00;
            uint8_t addrHi = 0x00;
        };
        BankSelect select;


        
};