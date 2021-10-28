#pragma once
#include <cstdint>
#include <array>

#include "Cpu.h"
#include "Ppu.h"
#include "Cartridge.h"

#include <iostream>
#include <memory>
#include <string>


class Bus
{
    public:
        Bus();
        ~Bus();

    public:
        // Devices on Bus
        Cpu cpu;                           // CPU
        uint8_t cpuRam[2048];              // RAM
        Ppu ppu;                           // PPU
        std::shared_ptr<Cartridge> cart;   // Cartridge

        uint8_t controllers[2] = {0};            // Controllers
        uint8_t controllerState[2] = {0};        // Status of Controllers
        bool controllerPoll = false;
        uint8_t userInput = 0;             // User input for Controller Polls

        bool DMA_access = false;
        bool continue_transfer = false;
        uint8_t DMA_transferData;
        uint8_t DMA_addr;
        uint8_t DMA_offset = 0x00;
        int DMA_cycles;


        // Bus read & write
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr, bool bReadOnly = false);

        // System Functions
        void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
        void systemReset();
        void systemClock();
        void setController(uint8_t bits, uint8_t num);
        uint8_t getController(uint8_t num);

        unsigned long NES_SystemClock = 0;

        // Debuging operators
        uint8_t bus_data = 0x00;
        uint16_t bus_addr = 0x0000;
        string bus_accessType;
};