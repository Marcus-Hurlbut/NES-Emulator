#pragma once
#include <stdio.h>
#include <array>
#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

class Mapper
{
    public:
        Mapper(uint8_t PRGsize, uint8_t CHRsize);
        ~Mapper();

    public:
        // Communication with Bus
        virtual bool mappedPrgRead(uint16_t addr, uint32_t &mappedAddr) = 0;
        virtual bool mappedPrgWrite(uint16_t addr,  uint32_t &mappedAddr, uint8_t data) = 0;

        // Communication with PPU Bus
        virtual bool mappedChrRead(uint16_t addr, uint32_t &mappedAddr) = 0;
        virtual bool mappedChrWrite(uint16_t addr,  uint32_t &mappedAddr) = 0;

        virtual void reset() = 0;

    protected:
        uint8_t nPRGBanks;
        uint8_t nCHRBanks;
};
