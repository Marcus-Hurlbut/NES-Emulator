#include "SDL2/SDL.h"
#include "SDL_ttf.h"

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


#include "Bus.h"
#include "Cpu.h"
#include "Ppu.h"

#define SDL_MAIN_HANDLED
#define LOGICAL_WIDTH 256
#define LOGICAL_HEIGHT 240
#define WIDTH 2560
#define HEIGHT 1300

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
        Bus nes;
        std::shared_ptr<Cartridge> cart;
        int count = 0;
        int FPS = 0;
        Uint32 ticks = 0;
        Uint32 prevTick = 0;

        Uint32 frame_start = 0;
        Uint32 frame_end = 0;

        Uint32 marsSpeed = 0;
        Uint32 marsStart = 0;

        stringstream text_ss;
        uint32_t* leftPlaneBuffer = nullptr;
        uint32_t* rightPlaneBuffer = nullptr;


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

        SDL_Color Red = {255, 0, 0};
        SDL_Color Green = {0, 255, 0};
        SDL_Color Blue = {0, 0, 255};
        SDL_Color White = {255, 255, 255};
        SDL_Color Gold = {212, 175, 55};

        bool eventHandler();
        bool debugMode();
        bool debMode = false;
        

    private:
        uint8_t selectedPalette = 0;
        vector<PixelPlane> pix;
        SDL_Texture *pixelTexture;

        void init();

        inline void drawPatternTables();
        inline void resetPatternTables(uint8_t palette);
        inline void drawPalettes();
        inline void drawCpu();
        inline void drawRam();
        inline void drawPpu();
        inline void drawSystemComponents();
        void drawPixels();

        uint32_t aRGB(uint32_t R, uint32_t G, uint32_t B, uint32_t a);

        void drawScreen();
        void keyboardInput(SDL_Event &event);
        void getControllerState();
        void logWrite();

};