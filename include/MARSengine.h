#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <stdio.h>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <windows.h>
#include <algorithm>
#include <chrono>


#include "Bus.h"
#include "Cpu.h"
#include "Ppu.h"

#define SDL_MAIN_HANDLED
#define LOGICAL_WIDTH 256
#define LOGICAL_HEIGHT 240
#define WIDTH 2560
#define HEIGHT 1440

#define RIGHT_L_JUSTIFIED 2100
#define RIGHT_R_JUSTIFIED 2300

#define LEFT_L_JUSTIFIED 50
#define LEFT_R_JUSTIFIED 260

class MARS
{
    public:
        MARS();
        ~MARS();

    private:
        // NES System Components
        Bus nes;
        std::shared_ptr<Cartridge> cart;

        // Frame Counter Variables
        int count = 0;
        int FPS = 0;
        Uint32 cur_tick = 0;
        Uint32 prev_tick = 0;

        // Frame Timing Sync Variables
        std::chrono::high_resolution_clock::time_point frame_start;
        std::chrono::high_resolution_clock::time_point frame_elapsed;

        // Emulator Speed Variables
        Uint32 marsSpeed = 0;
        Uint32 marsStart = 0;

        stringstream text_ss;
        uint32_t* leftPlaneBuffer = nullptr;    // NES PPU Left Pattern Table buffer
        uint32_t* rightPlaneBuffer = nullptr;   // NES PPU Right Pattern Table buffer

    public:
        // SDL variables
        SDL_Event event;
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_Window *wind_debug;
        SDL_Renderer *rend_debug;

        SDL_Surface *surface;
        SDL_Texture *texture;
        SDL_Rect box;

        struct Patterns
        {
            // CPU Ram
            SDL_Surface *surface;
            SDL_Texture *texture;
            SDL_Rect box;
        };
        Patterns pat;

      
        std::string selectedConsole = "NES";
        std::string selectedGame = "";        
        
        struct Plane
        {
            vector<Ppu::Pixels> left;
            vector<Ppu::Pixels> right;
        };
        Plane plane;

        struct PixelPlane
        {
            uint8_t R;
            uint8_t G;
            uint8_t B;
            uint8_t X;
            uint8_t Y;
        }; PixelPlane pixel;

        enum NesController
        {
           A = (1 << 7),
           B = (1 << 6),
           SELECT = (1 << 5),
           START = (1 << 4),
           UP = (1 << 3),
           DOWN = (1 << 2),
           LEFT = (1 << 1),
           RIGHT = (1 << 0),
        };

        int X;
        int Y;
        int W;
        int H;

        // Text Font & Colors
        TTF_Font *lg_font;
        TTF_Font *sm_font;
        TTF_Font *styled_font;
        TTF_Font *space_font;

        // Rendering Colors
        SDL_Color Red = {255, 0, 0};
        SDL_Color Green = {0, 255, 0};
        SDL_Color Blue = {0, 0, 255};
        SDL_Color White = {255, 255, 255};
        SDL_Color Gold = {212, 175, 55};

        bool debMode = false;
        
    // NES Functions
    public: 
        bool eventHandler();                                // Emulator event handler

    private:
        uint8_t selectedPalette = 0;
        vector<PixelPlane> pix;
        SDL_Texture *pixelTexture;

        void init();                                        // Initialize SDL avariables function

        void drawPatternTables();                           // Draw NES Pattern tables
        inline void resetPatternTables(uint8_t palette);    // Reset Pattern Table Buffers
        void drawPalettes();                                // Draw NES Palette Colors
        void drawNesInfo();                                 // Draw NES CPU & PPU Register values
        void drawEngineInfo();                              // Draw System Information and Disassembly
        void drawNesFrame();                                // Draw NES Frame Pixels
        uint32_t aRGB(uint32_t R, uint32_t G, uint32_t B, uint32_t a);          
        bool keyboardInput(SDL_Event &event);
        void getControllerState();
        void logWrite();

        // Assistive Functions
        void renderTexture(TTF_Font* font, const char *text, SDL_Color &color);     // Renders a text texture
        inline const char* getHexValue(uint8_t num);                                // Get String Hex Value
        void renderScreen();                                                        // Render All Screen Components

        void syncClockSpeed();          // Synchronize to run at 60 FPS
        void calculateFrameRate();      // Calculate the real-time FPS 
        void calculateEmulatorSpeed();     // Calculate the Clock Speed of the Emulator

};