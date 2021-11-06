#include "Bus.h"

Bus::Bus() 
{
    // Connect CPU -> Bus
    cpu.ConnectBus(this);
}

Bus::~Bus(){}

void Bus::write(uint16_t addr, uint8_t data)
{
    // Set Bus contents for debugging modules
    bus_addr = addr;
    bus_data = data;

    // Cpu Addressable Range
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        cpuRam[addr & 0x07FF] = data;   // Mirror every 2KB's
    }

    // PPU Mapped Register Range
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        ppu.cpuWrite(addr & 0x0007, data);
    } 

    // DMA Range
    else if(addr == 0x4014)
    {
        DMA_access = true;
        DMA_offset = data;
    }

    // Controller 1 Range
    else if(addr == 0x4016)
    {
        // Strobes Input on all writes
        controllerState[0] = getController(0);
    }

    // Controller 2 Range
    else if(addr == 0x4017)
    {
        // Strobes Input on all writes
        controllerState[1] = getController(1);
    }

    // Cartridge Range
    else if(addr >= 0x4020 && addr <= 0xFFFF)
    {
        cart->prgWrite(addr, data);
    }
}


uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;

    // Set Bus contents for debugging modules
    bus_addr = addr;

    // Cpu Addressable Range
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        data = cpuRam[addr & 0x07FF];   // Mirror every 2KB's
    } 

    // PPU Mapped Register Range
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        data = ppu.cpuRead(addr & 0x0007);
    }

    // Controller 1 Mapped Addr
    else if (addr == 0x4016)
    {
        if((controllerState[0] & 0x80) > 0)
        {
            data = 0x01;
        }
        else
        {
            data = 0x00;
        }; 
        controllerState[0] <<= 1;
    }

    // Controller 2 Mapped Addr
    else if (addr == 0x4017)
    {
        if((controllerState[1] & 0x80) > 0)
        {
            data = 0x01;
        }
        else
        {
            data = 0x00;
        }; 
        controllerState[1] <<= 1;
    }

    // Cartridge Range
    else if(addr >= 0x4020 && addr <= 0xFFFF)
    {
        cart->prgRead(addr, data);
    };

    bus_data = data;

    return data;
}

void Bus::setController(uint8_t bits, uint8_t num)
{
    num &= 0x01;
    controllers[num] = bits;
}

uint8_t Bus::getController(uint8_t num)
{
    num &= 0x01;
    return controllers[num];
}


void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::systemReset()
{
    cart->reset();
    cpu.reset();
    ppu.reset();
    NES_SystemClock = 0;

    DMA_access = false;
    continue_transfer = false;
    DMA_transferData = 0x00;
    DMA_offset = 0x00;
    DMA_addr = 0x00;
    DMA_cycles = 513;
    
}

void Bus::systemClock()
{
    // Single Tick PPU
    ppu.tick();

    if (DMA_access && ((NES_SystemClock % 3) == 0))
    {
        // Wait to do transfer
        if (NES_SystemClock % 2 == 1 && continue_transfer == false)
        {
            continue_transfer = true;
            DMA_cycles = 513;
        }
        // Even Cycles - CPU reads data from bus
        else if(NES_SystemClock % 2 == 0 && continue_transfer == true)
        {
            DMA_transferData = cpu.transferDMA(DMA_offset, DMA_addr);
            DMA_cycles--;
        }
        // Odd Cycles - Data is written to OAM address pointer
        else if(NES_SystemClock % 2 == 1 && continue_transfer == true)
        {
            ppu.pOAM_addr[DMA_addr] = DMA_transferData;
            DMA_addr++;
            DMA_cycles--;
        };


        // End of DMA Transfer - reset values
        if(DMA_cycles == 1)
        {
            DMA_cycles = 513;
            continue_transfer = false;
            DMA_access = false;
        };
    }

    // Single CPU Tick
    else if ((NES_SystemClock % 3) == 0)
    {
        cpu.tick();
    };

    // Check for NMI Request
    if(ppu.NMI == true)
    {
        cpu.nmi();
        ppu.NMI = false;
    };

    if (ppu.renderDisassembly)
    {
        cpu.renderDisassembly = true;
    }
    else
    {
        cpu.renderDisassembly = false;
    };

    NES_SystemClock++;
}