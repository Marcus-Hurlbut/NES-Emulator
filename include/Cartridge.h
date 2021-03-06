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

#include "Mapper000.h"
#include "Mapper002.h"

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
        void chrRead(uint16_t addr, uint8_t &data);
        void chrWrite(uint16_t addr,  uint8_t data);

        // Unmapped PPU Read/Writes
        void unmappedRead(uint16_t addr, uint8_t &data);
        void unmappedWrite(uint16_t addr, uint8_t data);

        bool ImageValid();
        void reset();

        struct PpuTables
        {
            uint8_t name[2][1024];         // 2 Nametables
            uint8_t patterns[2][4096];     // Left & Right Planes          
        };
        PpuTables tbl;

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
