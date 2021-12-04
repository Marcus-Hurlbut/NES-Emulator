#include "MARSengine.h"

MARS::MARS() 
{
    init();
}

MARS::~MARS(){}

bool MARS::keyboardInput(SDL_Event &event)
{   
    // Get SDL/Interface Input
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
            break;

        // Display RAM Contents
        case SDLK_PAGEDOWN:
            //drawRam();
            break;
        
        // Quit Application
        case SDLK_ESCAPE:
            return false;

        default:
            break;
    }
    return true;
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

// Render a Text Texture (Assistive function)
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

// Get Hex Value in String Form (Assistive function)
inline const char* MARS::getHexValue(uint8_t num)
{
    return nes.cpu.log.formatHex(num);
}


// Overall Speed of the Emulator's frame rendering (before synchronizing to run at 60 FPS)
void MARS::calculateEmulatorSpeed()
{
    marsSpeed = SDL_GetTicks();
    marsSpeed = (marsSpeed - marsStart);
}

// Calulate number of frames iterated each second in application
void MARS::calculateFrameRate()
{
    // Current Tick count
    cur_tick = SDL_GetTicks();

    // Reset Count every Second
    if ((cur_tick - prev_tick) >= 1000)
    {
        prev_tick = cur_tick;
        FPS = count;
        count = 0;
    }
    // +1 to Frame Rate Count
    else
    {
        count++;
    };
}


// Synchronize Clock speed to Render at contant 60 FPS using Chrono
void MARS::syncClockSpeed()
{
    long time_span = 0;

    // 1 frame every 16.66 milliseconds - 16,666 microseconds
    while ((time_span) <= 16200)
    {
        frame_elapsed = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::microseconds>(frame_elapsed - frame_start).count();
    };
    
}


// Draws the NES system information text
void MARS::drawNesInfo()
{
    // CPU PC
    X = RIGHT_L_JUSTIFIED;
    Y = 650;
    W = 100;
    H = 35;
    renderTexture(styled_font, "PC:", Red);

    std::string s_PC;

    s_PC = getHexValue(((nes.cpu.reg.PC & 0xFF00) >> 8));
    s_PC += getHexValue(((nes.cpu.reg.PC) & 0x00FF));
    const char* pc = s_PC.c_str();
    X = RIGHT_R_JUSTIFIED;
    W += 50;
    renderTexture(lg_font, pc, White);

    // CPU Accumulator
    X = RIGHT_L_JUSTIFIED;
    Y = 700;
    W = 50;
    H = 35;
    renderTexture(styled_font, "A:", Red);
    const char *a = getHexValue(nes.cpu.reg.A);
    X = RIGHT_R_JUSTIFIED;
    Y = 700;
    W = 75;
    H = 35;
    renderTexture(lg_font, a, White);

    // CPU X Register Text
    X = RIGHT_L_JUSTIFIED;
    Y = 750;
    W = 50;
    H = 35;
    renderTexture(styled_font, "X:", Red);
    const char *x = getHexValue(nes.cpu.reg.X);
    X = RIGHT_R_JUSTIFIED;
    Y = 750;
    W = 75;
    H = 35;
    renderTexture(lg_font, x, White);

    // CPU Y Register 
    X = RIGHT_L_JUSTIFIED;
    Y = 800;
    W = 50;
    H = 35;
    renderTexture(styled_font, "Y:", Red);
    const char *y = getHexValue(nes.cpu.reg.Y);
    X = RIGHT_R_JUSTIFIED;
    Y = 800;
    W = 75;
    H = 35;
    renderTexture(lg_font, y, White);

    // CPU Stack Text
    X = RIGHT_L_JUSTIFIED;
    Y = 850;
    W = 125;
    H = 35;
    renderTexture(styled_font, "STACK:", Red);
    const char *stack = getHexValue(nes.cpu.reg.S);
    X = RIGHT_R_JUSTIFIED;
    Y = 850;
    W = 75;
    H = 35;
    renderTexture(lg_font, stack, White);

    // CPU Status Flag 
    X = RIGHT_L_JUSTIFIED;
    Y = 900;
    W = 125;
    H = 35;
    renderTexture(styled_font, "STATUS:", Red);
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

    // PPU Controller Register
    X = RIGHT_L_JUSTIFIED;
    Y = 450;
    W = 175;
    H = 35;
    renderTexture(styled_font, "CONTROLLER: ", Red);
    const char* controllerVal = getHexValue(nes.ppu.r.controller);
    X = RIGHT_R_JUSTIFIED;
    Y = 450;
    W = 75;
    H = 35;
    renderTexture(lg_font, controllerVal, White);

    // PPU Mask Register
    X = RIGHT_L_JUSTIFIED;
    Y = 500;
    W = 175;
    H = 35;
    renderTexture(styled_font, "MASK: ", Red);
    const char* maskVal = getHexValue(nes.ppu.r.mask);
    X = RIGHT_R_JUSTIFIED;
    Y = 500;
    W = 75;
    H = 35;
    renderTexture(lg_font, maskVal, White);

    // PPU Status Register
    X = RIGHT_L_JUSTIFIED;
    Y = 550;
    W = 175;
    H = 35;
    renderTexture(styled_font, "STATUS: ", Red);
    const char* statusVal = getHexValue(nes.ppu.r.status);
    X = RIGHT_R_JUSTIFIED;
    Y = 550;
    W = 75;
    H = 35;
    renderTexture(lg_font, statusVal, White);

    // Draw Global Counter Text
    X = RIGHT_L_JUSTIFIED;
    Y = 1000;
    W = 125;
    H = 35;
    renderTexture(styled_font, "CYCLE:", Red);
    std::string str2 = to_string(nes.NES_SystemClock);
    const char* gCountVal = str2.c_str();
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
    const char* instructVal = nes.cpu.log.getInstruction();
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

    
    const char* addrmode = nes.cpu.log.getAddrMode();
    X = RIGHT_R_JUSTIFIED;
    Y = 1100;
    W = 100;
    H = 35;
    renderTexture(lg_font, addrmode, White);
    
    
    // Draw Opcode & Operands
    X = RIGHT_L_JUSTIFIED;
    Y = 1150;
    W = 125;
    H = 35;
    renderTexture(styled_font, "OPCODES:", Red);
    
    
    const char* opcode = nes.cpu.log.getOpcode();
    X = RIGHT_R_JUSTIFIED;
    Y = 1150;
    W = 65;
    H = 35;
    renderTexture(lg_font, opcode, White);
    

    
    // Draw Disassembly Log
    X = 150;
    Y = 500;
    W = 250;
    H = 35;
    renderTexture(styled_font, "- DISASSEMBLY -", Red);
    
    X = LEFT_L_JUSTIFIED;
    Y = 575;
    W = 425;
    H = 35;
     
    auto que = nes.cpu.log.getDisassemblyQueue();

    for (auto i = 0; i != que.size(); i++)
    {
        const char* disassembly = que[i].c_str();
        renderTexture(sm_font, disassembly, White);
        Y += 35;
    };   
}

// Draws the Emulator system information text
void MARS::drawEngineInfo()
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

    // Draw System Engine Clock Speed
    X = RIGHT_L_JUSTIFIED;
    Y = 350;
    W = 120;
    H = 35;
    renderTexture(styled_font, "CLOCK SPEED", Red);
    sFps = std::to_string((marsSpeed));
    const char *mars_clock = sFps.c_str();
    X = RIGHT_R_JUSTIFIED - 50;
    Y = 350;
    W = 75;
    H = 35;
    renderTexture(lg_font, mars_clock, White);

}

// Draws Each frame for NES screen
void MARS::drawNesFrame()
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

// Places Color info in 32 bit buffer (Assistive function)
uint32_t MARS::aRGB(uint32_t R, uint32_t G, uint32_t B, uint32_t a)
{
    uint32_t color = (a << 24) | (R << 16) | (G << 8) | B;
    return color;
}

// Draw the 2 NES Pattern Tables 
void MARS::drawPatternTables()
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
                // Map buffer addresss to the X & Y Coordinates
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

// Reset the Pixel Plane buffer for the patterns
inline void MARS::resetPatternTables(uint8_t palette)
{
    plane.left = nes.ppu.getPatternTables(0, palette);
    nes.ppu.patterns.clear();

    plane.right = nes.ppu.getPatternTables(1, palette);
    nes.ppu.patterns.clear();
}

// Render the NES Palette Colors stored in RAM
void MARS::drawPalettes()
{
    auto palettes = nes.ppu.getPalettes();

    int i = 0;
    X = RIGHT_L_JUSTIFIED ;
    Y = 230;
    W = 20;
    H = 20;
    int indx = 0;

    for (auto j = palettes.begin(); j != palettes.end(); j++)
    {

        SDL_SetRenderDrawColor(renderer, palettes[i].red, palettes[i].green, palettes[i].blue, 255);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;
        SDL_RenderFillRect(renderer, &box);

        if (indx == 15)
        {
            Y += 25;
            X = RIGHT_L_JUSTIFIED;
        }
        else if ((indx + 1) % 4 == 0)
        {
            X += 40;
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

    if(nes.cpu.new_instruction == true && ((nes.NES_SystemClock % 3) == 0))
    {       
        if(wLog.is_open())
        {
            //wLog << disassembly << "\t";
            //wLog << std::hex << (int)nes.cpu.d.get_opcode;
            //wLog << " ";
            //wLog << nes.cpu.d.get_opcode_instruction[0];
            //wLog << " ";
            //wLog << nes.cpu.d.get_opcode_instruction[1];

            //wLog << "\t\t A:";  wLog << std::hex << (int) nes.cpu.reg.A;
            //wLog << " X:";    wLog << std::hex << (int) nes.cpu.reg.X;
            //wLog << " Y:";    wLog << std::hex << (int) nes.cpu.reg.Y;
            //wLog << " P:";    wLog << std::hex << (int) nes.cpu.reg.P;
            //wLog << " SP:";   wLog << std::hex << (int) nes.cpu.reg.S;

            //wLog << "\t\t DISASSEMBLY: ";  wLog << nes.cpu.log.getDisassemblyQueue(0);
            //wLog << " OPS: ";    wLog << nes.cpu.log.getOps();


            wLog << "\n";
            wLog.close();
        };
    };
    
}

void MARS::renderScreen()
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
        
    // Draw Pixels
    drawNesFrame();
    drawPalettes();
    drawPatternTables();

    // Draw Text Information
    drawNesInfo();
    drawEngineInfo();

    SDL_RenderPresent(renderer);
}


bool MARS::eventHandler()
{  
    bool On = true;
    debMode = false;

    while (On)
    {
        // Start Timer for Frame Sync & Emulator clock Speed
        frame_start = std::chrono::high_resolution_clock::now();

        marsStart = SDL_GetTicks();

        // Clock NES for entire frame 
        while(nes.ppu.frameComplete == false)
        {
            nes.systemClock();
            //logWrite();  // -- uncomment for txt log -- 
        };

        // Render Frame & get User Input
        if(nes.ppu.frameComplete == true)
        {
            // User Controls Handling
            if (SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    // Handle Application Exit
                    case SDL_QUIT:
                        On = false;
                        break;
                    
                    // Handle User Input
                    case SDL_KEYDOWN:
                        if (!keyboardInput(event))
                        {
                            On = false;
                        }
                        break;
                    
                    default:
                        break;
                };
            };
            // Get NES Controller Input
            getControllerState();

            // Calculate Framerate
            calculateFrameRate();

            // Render Screen
            renderScreen();

            // Calculate Emulator Speed
            calculateEmulatorSpeed();

            // Sync to run at constant 60 FPS
            syncClockSpeed();
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
    window = SDL_CreateWindow("MARS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0); // SDL_WINDOW_FULLSCREEN
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