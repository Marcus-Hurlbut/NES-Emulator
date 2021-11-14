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

        // CPU Banks ($8000-$FFFF) - Selects 16 KB PRG ROM bank for CPU
        struct CPUBank
        {
            uint8_t A = 0x00;  // CPU $8000-$BFFF: 16 KB switchable PRG ROM bank
            uint8_t B = 0x00;  // CPU $C000-$FFFF: 16 KB PRG ROM bank, fixed to the last bank
        };
        CPUBank cpuBank;

        enum UxROM
        {
            PRG_ROM_WINDOW = 16384,
            PRG_ROM_BANK_SIZE = 16383,
            CHR_CAPACITY = 8192,
            CHR_ROM_BANK_SIZE = 8191
        };


        
};