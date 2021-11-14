// Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
#include "Cartridge.h"

Cartridge::Cartridge(const std::string &FileName)
{

    std::ifstream file;
    imageValid = false;

    // Open ROM file
    file.open(FileName, std::ifstream::binary);

    if(file.is_open())
    {
        // Read File Header
        file.read((char*) &header, sizeof(FileHeader));
        

        // Skip Trainer Data
        if(header.mapper & 0x04)
        {
            file.seekg(512, std::ios_base::cur);
        };

        // Determine the File format
        if((header.mapper2_0 & 0x0C) == 0x08)
        {
            file_format = NES20;
        }
        else
        {
            file_format = iNES;
        };

        // iNES file format
        if (file_format == iNES)
        {
            nPRGBanks = header.prg_rom_chunks;

            // Set size of PRG & CHR data
            PRG_Memory.resize((nPRGBanks) * 16384);
            file.read((char*)PRG_Memory.data(), PRG_Memory.size());

            nCHRBanks = header.chr_rom_chunks;
            if (nCHRBanks == 0)
            {
                // Create CHR RAM
                CHR_Memory.resize(8192);
            }
            else
            {
                // Allocate for ROM
                CHR_Memory.resize(nCHRBanks * 8192);
            };
            file.read((char*)CHR_Memory.data(), CHR_Memory.size());
        }

        // NES2.0 file format
        else if (file_format == NES20)
        {
            // Resizing Number of PRG Banks
            nPRGBanks = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
            PRG_Memory.resize(nPRGBanks * 16384);
            file.read((char*) CHR_Memory.data(), CHR_Memory.size());

            // Resizing Number of CHR Banks
            nCHRBanks = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;
            CHR_Memory.resize(nCHRBanks * 8192);
            file.read((char*) CHR_Memory.data(), CHR_Memory.size());

        };

        // Determine Mirror type
        if(header.mapper & 0x01) 
        {
            mirror = VERTICAL;
        }
        else
        {
            mirror = HORIZONTAL;
        };

        // Get Mapper ID from Header
        nMapperID = ((header.mapper2_0 >> 4) << 4) | (header.mapper >> 4);

        // Point to Correct Mapper
        switch(nMapperID)
        {
            case 0:
                pMapper = std::make_shared<Mapper000>(nPRGBanks, nCHRBanks);
                break;

            case 1:
                break;

            case 2:
                pMapper = std::make_shared<Mapper002>(nPRGBanks, nCHRBanks);
                break;
        };
        imageValid = true;
        file.close();
    };
}

Cartridge::~Cartridge(){}

bool Cartridge::ImageValid()
{
	return imageValid;
}


void Cartridge::reset()
{
    if (pMapper != nullptr)
		pMapper->reset();
}


void Cartridge::prgRead(uint16_t addr, uint8_t &data)
{
    uint32_t mappedAddr = 0;

    // Read Mapped address from specific Mapper
    if(pMapper->mappedPrgRead(addr, mappedAddr))
    {
        if (mappedAddr != 0xFFFFFFFF)
        {
            data = PRG_Memory[mappedAddr];
        };
    };
}


void Cartridge::prgWrite(uint16_t addr,  uint8_t data)
{
    uint32_t mappedAddr = 0;

    // Write Mapped address from specific Mapper
    if(pMapper->mappedPrgWrite(addr, mappedAddr, data))
    {
        if (mappedAddr != 0xFFFFFFFF)
        {
            PRG_Memory[mappedAddr] = data;
        };
    };
}


void Cartridge::chrRead(uint16_t addr, uint8_t &data)
{
    uint32_t mappedAddr = 0;

    // Read from Mapped address from Mapper
    if(pMapper->mappedChrRead(addr, mappedAddr))
    {
        data = CHR_Memory[mappedAddr];
    }
    // Read from Un-Mapped Addr
    else
    {
        unmappedRead(addr, data);
    };
}


void Cartridge::chrWrite(uint16_t addr,  uint8_t data)
{
    uint32_t mappedAddr = 0;

    // Write to Mapped address from Mapper
    if(pMapper->mappedChrWrite(addr, mappedAddr))
    {
        CHR_Memory[mappedAddr] = data;
    }
    // Write to Un-Mapped Addr
    else
    {
        unmappedWrite(addr, data);
    };
}



void Cartridge::unmappedRead(uint16_t addr, uint8_t &data)
{
    switch (addr)
    {   
        // Pattern Tables
        case 0x0000 ... 0x1FFF:
            data = tbl.patterns[((addr & 0x1000) & 0x2000) > 0][addr & 0x0FFF];
            break;

        // Name Tables
        case 0x2000 ... 0x3EFF:
            addr &= 0x0FFF;

            // Vertical Mirror
            if(mirror == VERTICAL)
            {
                switch (addr)
                {
                    case 0x0000 ... 0x03FF:
                        data = tbl.name[0][addr & 0x03FF];
                        break;
                            
                    case 0x0400 ... 0x07FF:
                        data = tbl.name[1][addr & 0x03FF];
                        break;

                    case 0x0800 ... 0x0BFF:
                        data = tbl.name[0][addr & 0x03FF];
                        break;

                    case 0x0C00 ... 0x0FFF:
                        data = tbl.name[1][addr & 0x03FF];
                        break;
                };
            }
            // Horizontal Mirror
            else if (mirror == HORIZONTAL)
            {
                switch (addr)
                {
                    case 0x0000 ... 0x03FF:
                        data = tbl.name[0][addr & 0x03FF];
                        break;
                            
                    case 0x0400 ... 0x07FF:
                        data = tbl.name[0][addr & 0x03FF];
                        break;

                    case 0x0800 ... 0x0BFF:
                        data = tbl.name[1][addr & 0x03FF];
                        break;

                    case 0x0C00 ... 0x0FFF:
                        data = tbl.name[1][addr & 0x03FF];
                        break;
                };
            }               
            break;     
    };
}

void Cartridge::unmappedWrite(uint16_t addr, uint8_t data)
{
    switch (addr)
    {   
        // Pattern Tables
        case 0x0000 ... 0x1FFF:
            tbl.patterns[((addr & 0x1000) & 0x2000) > 0][addr & 0x0FFF] = data;
            break;

        // Name Tables
        case 0x2000 ... 0x3EFF:
            addr &= 0x0FFF;

            // Vertical Mirror
            if(mirror == VERTICAL)
            {
                switch (addr)
                {
                    case 0x0000 ... 0x03FF:
                        tbl.name[0][addr & 0x03FF] = data;
                        break;
                            
                    case 0x0400 ... 0x07FF:
                        tbl.name[1][addr & 0x03FF] = data;
                        break;

                    case 0x0800 ... 0x0BFF:
                        tbl.name[0][addr & 0x03FF] = data;
                        break;

                    case 0x0C00 ... 0x0FFF:
                        tbl.name[1][addr & 0x03FF] = data;
                        break;
                };
            }
            // Horizontal Mirror
            else if (mirror == HORIZONTAL)
            {
                switch (addr)
                {
                    case 0x0000 ... 0x03FF:
                        tbl.name[0][addr & 0x03FF] = data;
                        break;
                            
                    case 0x0400 ... 0x07FF:
                        tbl.name[0][addr & 0x03FF] = data;
                        break;

                    case 0x0800 ... 0x0BFF:
                        tbl.name[1][addr & 0x03FF] = data;
                        break;

                    case 0x0C00 ... 0x0FFF:
                        tbl.name[1][addr & 0x03FF] = data;
                        break;
                };
            }               
            break;
    };
}
