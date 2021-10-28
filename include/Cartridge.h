#pragma once
#include <stdio.h>
#include <array>
#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <memory>

#include "Mapper_000.h"

class Cartridge
{
    public:
        // Constructors
        Cartridge(const std::string &FileName);
        ~Cartridge();

        // Cartridge Address Space on CPU Memory Map
        void prgRead(uint16_t addr, uint8_t &data);
        void prgWrite(uint16_t addr,  uint8_t data);

        // Cartridge Mapping Override on PPU
        bool chrRead(uint16_t addr, uint8_t &data);
        bool chrWrite(uint16_t addr,  uint8_t data);

        bool ImageValid();
        void reset();

        enum MIRROR
        {
            HORIZONTAL,
            VERTICAL,
            ONESCREEN_LO,
            ONESCREEN_HI,
            
        } mirror = HORIZONTAL;

        enum FileFormat
        {
            iNES,
            NES20,    
        };

        bool imageValid;

        struct FileHeader
        {
            char constant[4];
            uint8_t prg_rom_chunks;
            uint8_t chr_rom_chunks;
            uint8_t mapper;
            uint8_t mapper2_0;
            uint8_t prg_ram_size;
            uint8_t TV_system;
            uint8_t prg_ram_pres;
            char padding[5];
        }; 
        FileHeader header;

    private:
        uint8_t nMapperID = 0;
	    uint8_t nPRGBanks;
	    uint8_t nCHRBanks;
        FileFormat file_format;

        std::shared_ptr<Mapper> pMapper;
        std::vector<uint8_t> PRG_Memory;
        std::vector<uint8_t> CHR_Memory;


            
        

};
