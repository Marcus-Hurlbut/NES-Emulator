#include "MARSengine.h"

MARS::MARS() 
{
    init();
}

MARS::~MARS(){}

void MARS::keyboardInput(SDL_Event &event)
{
    switch(event.key.keysym.sym)
    {
        // Shift Pattern Table Selector Right        
        case SDLK_RIGHT:
            if (selectedPalette < 0x07)
            {
                selectedPalette++;
            }
            else
            {
                selectedPalette = 0;
            }
            resetPatternTables(selectedPalette);
            break;
        
        // Shift Pattern Table Selector Left
        case SDLK_LEFT:
            if (selectedPalette == 0)
            {
                selectedPalette = 0x07;
            }
            else
            {
                selectedPalette--;
            }
            resetPatternTables(selectedPalette);
            break;

        // Reset NES
        case SDLK_r:
            nes.systemReset();
            SDL_RenderClear(rend_debug);
            drawScreen();


        // Display RAM Contents
        case SDLK_PAGEDOWN:
            //drawRam();
            break;
        
        // Quit Application
        case SDLK_ESCAPE:
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            break;

        default:
            break;

    }
}

void MARS::getControllerState()
{
    const uint8_t *controller = SDL_GetKeyboardState(NULL);
    uint8_t bits = 0x00;

    if (controller[SDL_SCANCODE_D])
    {
        bits |= RIGHT;
    };

    if (controller[SDL_SCANCODE_A])
    {
        bits |= LEFT;
    };

    if (controller[SDL_SCANCODE_S])
    {
        bits |= DOWN;
    };

    if (controller[SDL_SCANCODE_W])
    {
        bits |= UP;
    };

    if (controller[SDL_SCANCODE_E])
    {
        bits |= START;
    };

    if (controller[SDL_SCANCODE_Q])
    {
        bits |= SELECT;
    };

    if (controller[SDL_SCANCODE_K])
    {
        bits |= B;
    };

    if (controller[SDL_SCANCODE_M])
    {
        bits |= A;
    };
    bits &= 0xFF;
    nes.setController(bits, 0);
    nes.setController(bits, 1);
}

void MARS::renderTexture(TTF_Font* font, const char *text, SDL_Color &color)
{
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = X;
    box.y = Y;
    box.w = W;
    box.h = H;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


inline void MARS::drawCpu()
{
    // PC Text
    X = RIGHT_L_JUSTIFIED;
    Y = 650;
    W = 100;
    H = 35;
    renderTexture(styled_font, "PC:", Red);


    // PC Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << nes.cpu.reg.PC;
    const char *pc = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    W += 50;
    renderTexture(lg_font, pc, White);


    // Accumulator Text
    X = RIGHT_L_JUSTIFIED;
    Y = 700;
    W = 50;
    H = 35;
    renderTexture(styled_font, "A:", Red);

    // Accumulator Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.A;
    const char *a = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 700;
    W = 75;
    H = 35;
    renderTexture(lg_font, a, White);


    // X Register Text
    X = RIGHT_L_JUSTIFIED;
    Y = 750;
    W = 50;
    H = 35;
    renderTexture(styled_font, "X:", Red);

    // X Register Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.X;
    const char *x = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 750;
    W = 75;
    H = 35;
    renderTexture(lg_font, x, White);


    // Y Register Text
    X = RIGHT_L_JUSTIFIED;
    Y = 800;
    W = 50;
    H = 35;
    renderTexture(styled_font, "Y:", Red);

    // Y Register Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.Y;
    const char *y = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 800;
    W = 75;
    H = 35;
    renderTexture(lg_font, y, White);


    // Stack Text
    X = RIGHT_L_JUSTIFIED;
    Y = 850;
    W = 125;
    H = 35;
    renderTexture(styled_font, "STACK:", Red);

    // Stack Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.S;
    const char *stack = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 850;
    W = 75;
    H = 35;
    renderTexture(lg_font, stack, White);

    // Status Flag Text
    X = RIGHT_L_JUSTIFIED;
    Y = 900;
    W = 125;
    H = 35;
    renderTexture(styled_font, "STATUS:", Red);


    // Status Flag Values
    std::bitset<8> status = nes.cpu.reg.P;
    X = RIGHT_R_JUSTIFIED;
    Y = 900;
    W = 25;
    H = 35;
    
    int j = 7;
    for(int i = 0; i <= 8; i++)
    {
        std::string s_p = status.to_string();
        std::string s_p_name = "CZIDBUVN";
        char name_bit = s_p_name[j];
        s_p = s_p[i];
        s_p_name = name_bit;
        const char *statusVal = s_p_name.c_str();
        
        if(s_p == "0")
        {
            renderTexture(sm_font, statusVal, Red);
        }
        else
        {
            renderTexture(sm_font, statusVal, Green);
        }
        X += 25;
        j--;
    };
}


inline void MARS::drawPpu()
{

    // Controller Register Text
    X = RIGHT_L_JUSTIFIED;
    Y = 450;
    W = 175;
    H = 35;
    renderTexture(styled_font, "CONTROLLER: ", Red);

    // Controller Register Values
    text_ss.str(std::string());
    text_ss.clear();
    text_ss << std::hex << (int)nes.ppu.r.controller;;
    const char* controllerVal = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 450;
    W = 75;
    H = 35;
    renderTexture(lg_font, controllerVal, White);


    // Mask Register Text
    X = RIGHT_L_JUSTIFIED;
    Y = 500;
    W = 175;
    H = 35;
    renderTexture(styled_font, "MASK: ", Red);

    //  Mask Register Values
    text_ss.str(std::string());
    text_ss.clear();
    text_ss << std::hex << (int)nes.ppu.r.mask;;
    const char* maskVal = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 500;
    W = 75;
    H = 35;
    renderTexture(lg_font, maskVal, White);


    // Status Register
    X = RIGHT_L_JUSTIFIED;
    Y = 550;
    W = 175;
    H = 35;
    renderTexture(styled_font, "STATUS: ", Red);

    text_ss.str(std::string());
    text_ss.clear();
    text_ss << std::hex << (int)nes.ppu.r.status;;
    const char* statusVal = text_ss.str().c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 550;
    W = 75;
    H = 35;
    renderTexture(lg_font, statusVal, White);
}


inline void MARS::drawSystemComponents()
{
    int global_count = nes.NES_SystemClock;

    // Draw MARS Title
    X = 100;
    Y = 50;
    W = 350;
    H = 120;
    renderTexture(space_font, "M.A.R.S", Red);

    X = 130;
    Y = 200;
    W = 290;
    H = 45;
    renderTexture(styled_font, "Marcus's ARcade System", Red);


    // Draw Selected Console
    X = LEFT_L_JUSTIFIED;
    Y = 350;
    W = 200;
    H = 45;
    renderTexture(styled_font, "CONSOLE: ", Red);

    const char *console = selectedConsole.c_str();
    X = LEFT_R_JUSTIFIED;
    Y = 350;
    W = 150;
    H = 45;
    renderTexture(styled_font, console, White);


    // Draw Selected Game
    X= LEFT_L_JUSTIFIED;
    Y = 400;
    W = 200;
    H = 45;
    renderTexture(styled_font, "GAME: ", Red);

    const char *game = selectedGame.c_str();
    X = LEFT_R_JUSTIFIED;
    Y = 400;
    W = 200;
    H = 45;
    renderTexture(styled_font, game, White);

    // Draw Global Counter Text
    X = RIGHT_L_JUSTIFIED;
    Y = 1000;
    W = 125;
    H = 35;
    renderTexture(styled_font, "CYCLE:", Red);

    // Draw Global Counter Value
    std::string str2 = to_string(global_count);
    const char *gCountVal = str2.c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 1000;
    W = 150;
    H = 35;
    renderTexture(lg_font, gCountVal, White);


    // Draw Instruction
    X = RIGHT_L_JUSTIFIED;
    Y = 1050;
    W = 150;
    H = 35;
    renderTexture(styled_font, "INSTRUCTION:", Red);

    std::string instruction = nes.cpu.d.get_instruction;
    const char *instructVal = instruction.c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 1050;
    W = 100;
    H = 35;
    renderTexture(lg_font, instructVal, White);


    // Draw Address Mode
    X = RIGHT_L_JUSTIFIED;
    Y = 1100;
    W = 150;
    H = 35;
    renderTexture(styled_font, "ADDR MODE:", Red);

    std::string addrm = nes.cpu.d.get_addrmode;
    const char *addrmode = addrm.c_str();
    X = RIGHT_R_JUSTIFIED;
    Y = 1100;
    W = 100;
    H = 35;
    renderTexture(lg_font, addrmode, White);


    // Draw Opcodes Text
    X = RIGHT_L_JUSTIFIED;
    Y = 1150;
    W = 125;
    H = 35;
    renderTexture(styled_font, "OPCODES:", Red);

    // Draw Opcodes Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.d.get_opcode;
    text_ss << " " << nes.cpu.d.get_opcode_instruction[0] << " " << nes.cpu.d.get_opcode_instruction[1];
    const char *opcode = text_ss.str().c_str();
    
    X = RIGHT_R_JUSTIFIED;
    Y = 1150;
    W = 200;
    H = 35;
    renderTexture(lg_font, opcode, White);


    // Draw Disassembly Log
    X = 150;
    Y = 500;
    W = 250;
    H = 35;
    renderTexture(styled_font, "- DISASSEMBLY -", Red);

    auto vDisassembly = nes.cpu.d.disassembly;
    int j = 0;
    X = LEFT_L_JUSTIFIED;
    Y = 575;
    W = 425;
    H = 35;

    for(auto i = 0; i != vDisassembly.size(); i++)
    {
        const char* disassembly = vDisassembly[i].c_str();
        renderTexture(sm_font, disassembly, White);
        Y += 35;
        j++;
    }; 
    
    // Draw FPS
    X = RIGHT_L_JUSTIFIED;
    Y = 300;
    W = 90;
    H = 35;
    renderTexture(styled_font, "FPS", Red);
    
    std::string sFps = std::to_string(FPS);
    const char *fpsVal = sFps.c_str();
    X = RIGHT_R_JUSTIFIED - 50;
    Y = 300;
    W = 75;
    H = 35;
    renderTexture(lg_font, fpsVal, White);


    // Draw Nes Clock Speed Text
    X = RIGHT_L_JUSTIFIED;
    Y = 350;
    W = 120;
    H = 35;
    renderTexture(styled_font, "CLOCK SPEED", Red);
    

    // Draw Nes Clock Speed 
    marsSpeed = SDL_GetTicks();
    marsSpeed = (marsSpeed - marsStart);
    marsStart = SDL_GetTicks();
    sFps = std::to_string((marsSpeed));
    const char *mars_clock = sFps.c_str();

    X = RIGHT_R_JUSTIFIED - 50;
    Y = 350;
    W = 75;
    H = 35;
    renderTexture(lg_font, mars_clock, White);

}


void MARS::drawPixels()
{
    if (nes.ppu.frameComplete == true )
    {
        SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
        int32_t pitch = 0;

        // Pointer to the memory position in VRAM where Back Buffer texture lies
        uint32_t* pixelBuffer = nullptr;

        // Lock Memory to write Back Buffer image
        if (!SDL_LockTexture(pixelTexture, NULL, (void**)&pixelBuffer, &pitch))
        {
            // The pitch of the Back Buffer texture in VRAM must be divided by four bytes as it will always be a multiple of four
            pitch /= sizeof(uint32_t);

            // Fill texture with Pixels
            for (uint32_t i = 0; i < LOGICAL_WIDTH * LOGICAL_HEIGHT; i++)
            {
                pixelBuffer[i] = aRGB(nes.ppu.screenPixels[i].red, nes.ppu.screenPixels[i].green, nes.ppu.screenPixels[i].blue, 255);
            };
            // Unlock the texture in VRAM to let the GPU know we are done writing to it
            SDL_UnlockTexture(pixelTexture);
            SDL_RenderCopy(renderer, pixelTexture, NULL, NULL);
        };

        SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
        if (nes.ppu.frameComplete)
        {
            nes.ppu.frameComplete = false;
        };
    };
}


uint32_t MARS::aRGB(uint32_t R, uint32_t G, uint32_t B, uint32_t a)
{
    uint32_t color = (a << 24) | (R << 16) | (G << 8) | B;
    return color;
}


inline void MARS::drawPatternTables()
{
    // Draw Left Plane Pattern Table
    uint32_t j = 0;

    leftPlaneBuffer = nullptr;
    rightPlaneBuffer = nullptr;
    int32_t location;
    int32_t pitch = 0;
    pat.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 128, 128);

    // Lock Memory to write Back Buffer image
    if (!SDL_LockTexture(pat.texture, NULL, (void**)&leftPlaneBuffer, &pitch))
    {
        pitch /= sizeof(uint32_t);
        j = 0;

        for (auto i = plane.left.begin(); i != plane.left.end(); i++)
        {
            if (plane.left[j].Y == 0)
            {
                if (plane.left[j].Y == 0 && plane.left[j].X == 0)
                {
                    location = 0;
                }
                else if (plane.left[j].Y == 0)
                {
                    location = plane.left[j].X;
                };
            }
            else
            {
                location =  (plane.left[j].Y * 128) + plane.left[j].X;
            };
            leftPlaneBuffer[location] = aRGB(plane.left[j].red, plane.left[j].green, plane.left[j].blue, 255);
            ++j;
        };
        pat.box.x = RIGHT_L_JUSTIFIED;
        pat.box.y = 50;
        pat.box.w = 128;
        pat.box.h = 128;
        SDL_UnlockTexture(pat.texture);
        SDL_RenderCopy(renderer, pat.texture, NULL, &pat.box);
        SDL_DestroyTexture(pat.texture);
    };

    // Draw Right Plane Pattern Table
    pat.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 128, 128);
    pitch = 0;
    if (!SDL_LockTexture(pat.texture, NULL, (void**)&rightPlaneBuffer, &pitch))
    {
        pitch /= sizeof(uint32_t);
        j = 0;

        for (auto i = plane.right.begin(); i != plane.right.end(); i++)
        {
            if (plane.right[j].Y == 0)
            {
                if (plane.right[j].Y == 0 && plane.right[j].X == 0)
                {
                    location = 0;
                }
                else if (plane.right[j].Y == 0)
                {
                    location = plane.right[j].X;
                };
            }
            else
            {
                location =  (plane.right[j].Y * 128) + plane.right[j].X;
            };
            rightPlaneBuffer[location] = aRGB(plane.right[j].red, plane.right[j].green, plane.right[j].blue, 255);
            ++j;
        };
        pat.box.x = RIGHT_L_JUSTIFIED + 150;
        pat.box.y = 50;
        pat.box.w = 128;
        pat.box.h = 128;
        SDL_UnlockTexture(pat.texture);
        SDL_RenderCopy(renderer, pat.texture, NULL, &pat.box);
        SDL_DestroyTexture(pat.texture);
    };

}

void MARS::resetPatternTables(uint8_t palette)
{
    plane.left = nes.ppu.getPatternTables(0, palette);
    nes.ppu.patterns.clear();

    plane.right = nes.ppu.getPatternTables(1, palette);
    nes.ppu.patterns.clear();
}

inline void MARS::drawPalettes()
{
    auto palettes = nes.ppu.getPalettes();

    int i = 0;
    X = RIGHT_L_JUSTIFIED ;
    Y = 230;
    W = 20;
    H = 20;
    int indx = 1;

    for (auto j = palettes.begin(); j != palettes.end(); j++)
    {

        SDL_SetRenderDrawColor(renderer, palettes[i].red, palettes[i].green, palettes[i].blue, 255);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;
        SDL_RenderFillRect(renderer, &box);
        if((indx == 4) || (indx == 8) || (indx == 12) || (indx == 20) || (indx == 24) || (indx == 28))
        {
            X += 40;
        }

        else if (indx == 16)
        {
            Y += 25;
            X = RIGHT_L_JUSTIFIED;
        }
        else
        {
            X += 20;
        }

        i++;
        indx++;
    }
}

void MARS::logWrite()
{
    std::string disassembly = nes.cpu.thisDis;
    ofstream wLog("C:\\Users\\Thugs4Less\\Desktop\\Program Projects\\NES\\log\\disassembly.txt", std::ofstream::app);

    if(nes.cpu.newInstruction == true && ((nes.NES_SystemClock % 3) == 0))
    {       
        if(wLog.is_open())
        {
            wLog << disassembly << "\t";
            wLog << std::hex << (int)nes.cpu.d.get_opcode;
            wLog << " ";
            wLog << nes.cpu.d.get_opcode_instruction[0];
            wLog << " ";
            wLog << nes.cpu.d.get_opcode_instruction[1];

            wLog << "\t\t A:";  wLog << std::hex << (int) nes.cpu.reg.A;
            wLog << " X:";    wLog << std::hex << (int) nes.cpu.reg.X;
            wLog << " Y:";    wLog << std::hex << (int) nes.cpu.reg.Y;
            wLog << " P:";    wLog << std::hex << (int) nes.cpu.reg.P;
            wLog << " SP:";   wLog << std::hex << (int) nes.cpu.reg.S;

            wLog << "\n";
            wLog.close();
        };
    };
    
}

void MARS::drawScreen()
{
    if(nes.ppu.frameComplete == true || debMode == true)
    {
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
                
        drawPixels();
        drawCpu();
        drawPpu();
        drawPalettes();
        drawPatternTables();
        drawSystemComponents();

        SDL_RenderPresent(renderer);
    };
}


bool MARS::eventHandler()
{  
    bool On = true;
    debMode = false;

    while (On)
    {
        frame_start = SDL_GetTicks();

        // Clock an entire frame
        while(nes.ppu.frameComplete == false)
        {
            nes.systemClock();
            // logWrite();
        };

        // Render and get User Input in between Frames
        if(nes.ppu.frameComplete == true)
        {

            if (SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        On = false;
                        break;

                    case SDL_KEYDOWN:
                        keyboardInput(event);
                        break;
                    
                    default:
                        break;
                };
            };

            // Calculate Framerate
            ticks = SDL_GetTicks();
            if ((ticks - prevTick) > 1000) 
            {
                prevTick = ticks;
                FPS = count;
                count = 0;
            }
            else
            {
                count++;
            };

            // Gets Input for NES controller
            getControllerState();

            // Finally Draw the frame
            drawScreen();
            frame_end = SDL_GetTicks();

            // Set to 60 frames per second
            while((frame_end - frame_start) < 16)
            {
                frame_end = SDL_GetTicks();
            };
        };
    };
    SDL_DestroyRenderer(renderer);
    SDL_DestroyRenderer(rend_debug);
    return false;
}

void MARS::init()
{
    marsStart = SDL_GetTicks();

    // Initialize SDL 
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    // Main window & Main Renderer
    window = SDL_CreateWindow("MARS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Debug window & Debug Renderer
    //wind_debug = SDL_CreateWindow("MARS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1800, 1000, 0);
    //rend_debug = SDL_CreateRenderer(wind_debug, -1, SDL_RENDERER_ACCELERATED);

    pixelTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // Set Font Elements
    lg_font = TTF_OpenFont("fonts/Anonymous.ttf", 80);
    TTF_SetFontStyle(lg_font, TTF_STYLE_BOLD);

    styled_font = TTF_OpenFont("fonts/NoSurrenderItalic.ttf", 80);
    sm_font = TTF_OpenFont("fonts/Urial.ttf", 80);
    space_font = TTF_OpenFont("fonts/Gatsby.ttf", 80);

    uint8_t console = 4;
    switch (console)
    {
        case 1:
            cart = std::make_shared<Cartridge>("roms/Nestest.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "NES Test";
            break;

        case 2:
            cart = std::make_shared<Cartridge>("roms/DonkeyKong.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Donkey Kong";
            break;

        case 3:
            cart = std::make_shared<Cartridge>("roms/IceClimber.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Ice Climber";
            break;

        case 4:
            cart = std::make_shared<Cartridge>("roms/SuperMarioBros.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Super Mario Bros";
            break;

        case 5:
            cart = std::make_shared<Cartridge>("roms/Dr.Mario.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Dr. Mario";
            break;

        case 6:
            cart = std::make_shared<Cartridge>("roms/SpaceInvaders.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Space Invaders";
            break;

        case 7:
            cart = std::make_shared<Cartridge>("roms/Pac-Man.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Pac-Man";
            break;
        case 8:
            cart = std::make_shared<Cartridge>("roms/Pinball.nes");
            if (!cart->ImageValid())
            {
			    SDL_Quit();
            };
            selectedGame = "Pinball";
            break;

        case 9:
            cart = std::make_shared<Cartridge>("roms/MegaMan.nes");
            if (!cart->ImageValid())
            {
                SDL_Quit();
            };
            selectedGame = "Mega Man";

        case 10:
            cart = std::make_shared<Cartridge>("roms/MetalGear.nes");
            if (!cart->ImageValid())
            {
                SDL_Quit();
            };
            selectedGame = "Metal Gear";            
        
        default:
            break;
    };

    nes.insertCartridge(cart);
    nes.systemReset();
    resetPatternTables(0);
}


int main(int argc, char* argv[])
{
    MARS mars;

    mars.eventHandler();    // Main Event Handler
    SDL_Quit();

    return 0;
}

// COMPILE
// mingw32-make -f Makefile
// g++ *.cpp -IC:\SDL_32bit\i686-w64-mingw32\include\SDL2 -IC:\SDL_ttf\include\SDL2 -LC:\SDL_32bit\i686-w64-mingw32\lib -LC:\SDL_ttf\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o mars.exe