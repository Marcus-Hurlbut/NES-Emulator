// Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
#include "Mapper_000.h"


Mapper000::Mapper000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{

}

Mapper000::~Mapper000()
{

}

void Mapper000::reset()
{

}

bool Mapper000::mappedPrgRead(uint16_t addr, uint32_t &mappedAddr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		if (nPRGBanks > 1)
		{
			mappedAddr = addr & 0x7FFF;
		}
		else
		{
			mappedAddr = addr & 0x3FFF;
		};
		return true;
	};

	return false;
}

bool Mapper000::mappedPrgWrite(uint16_t addr, uint32_t &mappedAddr, uint8_t data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		// Can only have 1 or 2 banks
		if (nPRGBanks > 1)
		{
			mappedAddr = addr & 0x7FFF;
		}
		else
		{
			mappedAddr = addr & 0x3FFF;
		};
		return true;
	};

	return false;
}

bool Mapper000::mappedChrRead(uint16_t addr, uint32_t &mappedAddr)
{
	// Maps to the Pattern Tables in ROM
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		mappedAddr = addr;
		return true;
	};

	return false;
}

bool Mapper000::mappedChrWrite(uint16_t addr, uint32_t &mappedAddr)
{
	return false;
}