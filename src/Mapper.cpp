// Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
#include "Mapper.h"

// Mapper constructor for overload
Mapper::Mapper(uint8_t PRG_size, uint8_t CHR_size)
{
    nPRGBanks = PRG_size;
    nCHRBanks = CHR_size;
	reset();
}

Mapper::~Mapper()
{

}

void Mapper::reset()
{

}