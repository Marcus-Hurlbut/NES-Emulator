#pragma once
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <array>
#include <vector>
#include <bitset>
#include <memory>
#include <string>
#include <cstring>

#include "Cartridge.h"


class Ppu
{
    public:
        Ppu();
        ~Ppu();

        // PPU Memory Bus
        std::array <uint8_t, 2048> ppuRam;     // PPU RAM
        std::shared_ptr<Cartridge> cart;
        void connectCartridge(const std::shared_ptr<Cartridge> &cartridge);


        // ---------------------------- Memory-mapped registers (CPU) ---------------------------- //
        // PPU Registers
        struct MappedRegisters
        {
            uint8_t controller = 0;         // Controller - Various flags controlling PPU operation
            uint8_t mask = 0;               // Mask - Controls the rendering of sprites and backgrounds, as well as colour effects
            uint8_t status = 0;             // Status - Reflects the state of various functions inside the PPU. It is often used for determining timing 
                                               // - Read resets write pair for $2005/$2006

            uint8_t oam_addr = 0;           // OAM read/write address
            uint8_t oam_data = 0;           // OAM data read/write
            uint8_t scroll = 0;             // fine scroll position (two writes: X scroll, Y scroll)
            uint8_t ppu_addr = 0;           // PPU read/write address (two writes: most significant byte, least significant byte)
            uint8_t ppu_data = 0;            // PPU data read/write

            uint8_t oam_dma = 0;            // OAM DMA high address
        };
        MappedRegisters r;

    private:

        // PPU Bit Operations
        enum ControllerFlags
        {
            n = (1 << 0),               // nametable select (n)
            N = (1 << 1),               // nametable select (N)
            I = (1 << 2),               // increment mode (I)  
            S = (1 << 3),               // sprite tile select (S)
            B = (1 << 4),               // background tile select (B)
            H = (1 << 5),               // sprite height (H)
            P = (1 << 6),               // PPU master/slave (P)
            V = (1 << 7)                // NMI enable (V)
        };
        enum MaskBits
        {
            G = (1 << 0),               // greyscale (G)
            m = (1 << 1),               // background left column enable (m)
            M = (1 << 2),               // sprite left column enable (M)
            b = (1 << 3),               // Background enable (b)
            s = (1 << 4),               // Sprite enable (s)
            R = (1 << 5),               // Color emphasis (RGB)
            Gr = (1 << 6),
            Bl = (1 << 7)
        };
        enum StatusBits
        {
            U = (1 << 0) && (1 << 1) && (1 << 2) && (1 << 3) && (1 << 4),
            O = (1 << 5),       // Sprite Overflow
            SO = (1 << 6),      // Sprite 0 Hit (S)
            v = (1 << 7)        // Vertical blank (V)
        };
        
        // Get & Set CPU mapped, PPU Registers Bits
        inline uint8_t getControllerFlags(ControllerFlags cBits);     // Get Controller Bits
        inline void setControllerFlags(ControllerFlags cBits, bool mode);   // Set or clear Controller bit values

        inline uint8_t getMaskBits(MaskBits mBits);                 // Get Mask Bits
        inline void setMaskBits(MaskBits, bool mode);               // Set or clear P bit values

        inline uint8_t getStatusBits(StatusBits sBits);             // Get Status Bits
        inline void setStatusBits(StatusBits, bool mode);           // Set or clear P bit values
    

    

    public:
        // ---------------------------- PPU Tables ---------------------------- //
        struct RGB
        {
            uint8_t red = 255;
            uint8_t green = 0;
            uint8_t blue = 0;
        };

        // Pixel info to be rendered
        struct Pixels
        {
            uint8_t red = 0;
            uint8_t green = 0;
            uint8_t blue = 0;
            uint8_t X = 0;
            uint8_t Y = 0;
        };

        // Pixels for Engine
        std::vector <Pixels> patterns;      // Pattern Table Pixels
        Pixels screenPixels[61440] = {0};   // Screen Pixels
        uint32_t pixel_counter = 0;         // Iterator for Pixel Array
        Pixels renderPixel = {0};           // Individual Pixel info
        std::vector <RGB> palettes;         // Palette Pixels

        // Name Tables - 32x32 bytes(256x240 bits on screen) : $2000 -> $3EFF
        struct Tables
        {
            uint8_t name[2][1024];         // 2 Nametables
            uint8_t patterns[2][4096];     // Left & Right Planes
            std::vector <RGB> colors;      // 2C02 Color palette of RGB values
            uint8_t palettesMem[32];       // Memory locations for sprites & background
        };
        Tables tbl;


        // ---------------------------- Screen Rendering ---------------------------- //
        int16_t scanLine = 0;
        int16_t cycles = 0;
        bool NMI = false;

        // ---------------------------- Internal Registers ---------------------------- //    
        // Internal Registers - Addresses that the PPU reads during background rendering
        enum VRAMRegisters
        {
            COARSE_X = 0x001F,
            COARSE_Y = 0x03E0,
            NAMETABLE_X = 0x0400,
            NAMETABLE_Y = 0x0800,
            FINE_Y = 0x7000,
            VRAM_REG = 0x7FFF
        };

        inline uint16_t getVRAM(VRAMRegisters vreg);
        inline void setVRAM(VRAMRegisters vreg, uint16_t vreg_data);

        inline uint16_t getTVRAM(VRAMRegisters vreg);
        inline void setTVRAM(VRAMRegisters vreg, uint16_t vreg_data);

        uint16_t currVRAM = 0x0000;
        uint16_t tempVRAM = 0x0000;
        uint8_t fine_X = 0x00;
        uint8_t toggle = 0x00;
        uint8_t buffer = 0x00;

        // ---------------------------- Background Rendering ---------------------------- //
        // PPU background fetched values
        struct Fetch
        {
            uint8_t tileNameTbl;
            uint8_t tileAttr;
            uint8_t patternLeft;
            uint8_t patternRight;
        };
        Fetch fetched;
        int iteration = 0;


        // PPU Shift Registers
        struct ShiftRegisters
        {
            uint16_t palAttribLow;
            uint16_t palAttribHi;
            uint16_t patternLeft;
            uint16_t patternRight;
        };
        ShiftRegisters shift;


        // ---------------------------- Sprite Rendering ---------------------------- //
        struct Sprite
        {
            uint8_t positionY;
            uint8_t tileIndex;
            uint8_t attributes;
            uint8_t positionX;
        };
        Sprite OAM [64];
        uint8_t *pOAM_addr = (uint8_t*) OAM;
        Sprite second_OAM[8];
        Sprite fetchedSprite;

        struct SpriteShifters
        {
            uint8_t lo_patternBit;
            uint8_t hi_patternBit;
            uint16_t lo_patternAddr;
            uint16_t hi_patternAddr;
            uint8_t counter;
            uint8_t latch;
        };
        SpriteShifters sprShift[8];

        uint8_t pOAM_counter = 0;        // Counts through elements in primary OAM (0 - 63)
        uint8_t sOAM_counter = 0;       // Counts through elements in secondary OAM (0 - 7)
        uint8_t sprites_found = 0;      // # of sprites found for next scanline
        uint8_t spritesThisLine = 0;
        uint8_t spr_init = 0;

        bool sprZeroPossible = false;
        bool sprZeroThisLine = false;

        bool sprZeroRendering = false;
        
        enum SpriteFlip
        {
            VERTICAL,
            HORIZONTAL,
            NORMAL
        } sprite_flip;

        
        // ---------------------------- PPU Functions ---------------------------- //

        // Read/Writes
        uint8_t ppuRead(uint16_t addr);
        void ppuWrite(uint16_t addr, uint8_t data);
        uint8_t cpuRead(uint16_t addr);
        void cpuWrite(uint16_t addr, uint8_t data);

        
        std::vector<Ppu::Pixels> getPatternTables(int plane, uint8_t pal);
        std::vector<Ppu::RGB> getPalettes();
        uint8_t palAddr = 0x00;

        Ppu::Pixels getScreenPixels();
        void setScreenPixels();


        // System Operations
        void tick();        
        void reset();
        bool frameComplete;
        bool renderDisassembly;

        // Scrolling Operations
        void incrementCoarseX();
        void incrementY();

        // Shift Register functions
        inline void strobeShiftRegisters();
        void updateBackgroundShiftRegisters();

        // Fetching functions
        void fetchBackground();
        void fetchSprites();

        // Sprite functions
        inline bool spriteRangeCheck();
        bool checkSprPriority(uint8_t bkg_pixel, uint8_t spr_pixel, bool spr_priority);
   
};
