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
        case SDLK_SPACE:
            if(debMode == true)
            {
                nes.systemClock();
                drawScreen();

            };
            break;
        
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

        case SDLK_DOWN:
            break;

        case SDLK_UP:
            break;

        case SDLK_p:
            drawPatternTables();
            break;

        case SDLK_r:
            nes.systemReset();
            SDL_RenderClear(rend_debug);
            drawScreen();

        // Render a Whole Frame
        case SDLK_RETURN:
            if (debMode == true)
            {
                while(nes.ppu.frameComplete == false)
                {
                    nes.systemClock();
                    logWrite();
                }
                drawScreen();
            }
            break;

        // Display RAM Contents
        case SDLK_PAGEDOWN:
            drawRam();
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


inline void MARS::drawCpu()
{
    X = RIGHT_L_JUSTIFIED;
    Y = 600;
    W = 100;
    H = 35;
    
    // PC Text
    const char *pcText = "PC:";
    surface = TTF_RenderText_Solid(styled_font, pcText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = X;
    box.y = Y;
    box.w = W;
    box.h = H;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // PC Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << nes.cpu.reg.PC;
    const char *pc = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, pc, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 600;
    box.w = 150;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Accumulator Text
    const char *aText = "A:";
    surface = TTF_RenderText_Solid(styled_font, aText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 650;
    box.w = 50;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Accumulator Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.A;
    const char *a = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, a, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 650;
    box.w = 75;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // X Register Text
    //std::string s_x = "X:";
    const char *xText = "X:";
    surface = TTF_RenderText_Solid(styled_font, xText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 700;
    box.w = 50;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // X Register Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.X;
    const char *x = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, x, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 700;
    box.w = 75;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);



    // Y Register Text
    const char *yText = "Y:";
    surface = TTF_RenderText_Solid(styled_font, yText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 750;
    box.w = 50;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Y Register Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.Y;
    const char *y = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, y, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 750;
    box.w = 75;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Stack Text
    const char *stackText = "STACK:";
    surface = TTF_RenderText_Solid(styled_font, stackText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 800;
    box.w = 125;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Stack Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.S;
    const char *stack = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, stack, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 800;
    box.w = 75;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);



    // Status Flag Text
    const char *statusText = "STATUS:";
    surface = TTF_RenderText_Solid(styled_font, statusText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 850;
    box.w = 125;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Status Flag Values
    std::bitset<8> status = nes.cpu.reg.P;
    X = RIGHT_R_JUSTIFIED;
    Y = 850;
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
            surface = TTF_RenderText_Solid(sm_font, statusVal, Red);
        }
        else
        {
            surface = TTF_RenderText_Solid(sm_font, statusVal, Green);
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;
        X += 25;
        j--;
        SDL_RenderCopy(renderer, texture, NULL, &box);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };
    /*
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.reg.P;
    const char *stat= text_ss.str().c_str();

    surface = TTF_RenderText_Solid(sm_font, stat, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = X + 10;
    box.y = Y;
    box.w = W;
    box.h = H;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    */
    // Render CPU Elements
    //SDL_RenderPresent(renderer);
}

inline void MARS::drawRam()
{
    // Get RAM contents from NES
    SDL_SetRenderDrawColor(rend_debug, 20, 20, 20, 255);
    SDL_RenderClear(rend_debug);

    // Secondary OAM Text
    const char *sOamText = "Secondary OAM:";
    surface = TTF_RenderText_Solid(lg_font, sOamText, Blue);
    texture = SDL_CreateTextureFromSurface(rend_debug, surface);
    box.x = 5;
    box.y = 25;
    box.w = 125;
    box.h = 25;
    SDL_RenderCopy(rend_debug, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    int ram_size = 8;
    std::stringstream ss;
    X = 5;
    Y = 80;
    W = 130;
    H = 20;

    // Place RAM into String
    for (int i = 0; i < ram_size; i++)
    {
        //auto ram = nes.ppu.tbl.name[0][i];
        ss.str( std::string() );
        ss.clear();

        ss << std::hex << (int) nes.ppu.second_OAM[i].positionY;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.second_OAM[i].tileIndex;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.second_OAM[i].attributes;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.second_OAM[i].positionX;
        ss << "  ";
        const char *sOamVal = ss.str().c_str();

        // Render RAM Contents
        surface = TTF_RenderText_Solid(sm_font, sOamVal, White);
        texture = SDL_CreateTextureFromSurface(rend_debug, surface);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;

        // Reset Y coordinate 
        X = 5;
        Y += 25;
        SDL_RenderCopy(rend_debug, texture, NULL, &box);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };

    // Primary OAM Data
    const char *pOamText = "Primary OAM Addresses:";
    surface = TTF_RenderText_Solid(lg_font, pOamText, Blue);
    texture = SDL_CreateTextureFromSurface(rend_debug, surface);
    box.x = 180;
    box.y = 25;
    box.w = 140;
    box.h = 25;
    SDL_RenderCopy(rend_debug, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    ram_size = 64;
    X = 180;
    Y = 55;
    W = 130;
    H = 20;

    // Place RAM into String
    for (int i = 0; i < ram_size; i++)
    {
        //auto ram = nes.ppu.tbl.name[0][i];
        ss.str( std::string() );
        ss.clear();

        ss << std::hex << (int) nes.ppu.OAM[i].positionY;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.OAM[i].tileIndex;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.OAM[i].attributes;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.OAM[i].positionX;
        ss << "  ";
        const char *pOamVal = ss.str().c_str();

        // Reset Y Coordinate
        if ((i % 8) == 0)
        {
            X = 180;
            Y += 25;
        };

        // Render RAM Contents
        surface = TTF_RenderText_Solid(sm_font, pOamVal, White);
        texture = SDL_CreateTextureFromSurface(rend_debug, surface);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;


        X += 160;
        SDL_RenderCopy(rend_debug, texture, NULL, &box);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };

    Y += 80;
    // Sprite Shift Register Data
    const char *sprshiftText = "Sprite Shift Registers:";
    surface = TTF_RenderText_Solid(lg_font, sprshiftText, Blue);
    texture = SDL_CreateTextureFromSurface(rend_debug, surface);
    box.x = 180;
    box.y = Y;
    box.w = 140;
    box.h = 25;
    SDL_RenderCopy(rend_debug, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    ram_size = 8;
    X = 180;
    Y += 30;
    W = 130;
    H = 20;

    // Place RAM into String
    for (int i = 0; i < ram_size; i++)
    {
        //auto ram = nes.ppu.tbl.name[0][i];
        ss.str( std::string() );
        ss.clear();

        ss << std::hex << (int) nes.ppu.sprShift[i].lo_patternBit;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.sprShift[i].hi_patternBit;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.sprShift[i].lo_patternAddr;
        ss << "  ";
        ss << std::hex << (int) nes.ppu.sprShift[i].hi_patternAddr;
        ss << "  ";
        const char *sprshiftVal = ss.str().c_str();

        // Reset Y Coordinate
        if ((i % 8) == 0)
        {
            X = 180;
            Y += 25;
        };

        // Render RAM Contents
        surface = TTF_RenderText_Solid(sm_font, sprshiftVal, White);
        texture = SDL_CreateTextureFromSurface(rend_debug, surface);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;


        X += 160;
        SDL_RenderCopy(rend_debug, texture, NULL, &box);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };
    SDL_RenderPresent(rend_debug);

}


inline void MARS::drawPpu()
{
    const char *cText = "CONTROLLER: ";
    const char *mText = "MASK: ";
    const char *sText = "STATUS: ";


    // Controller Register Text & Values
    surface = TTF_RenderText_Solid(styled_font, cText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 400;
    box.w = 175;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    X = RIGHT_R_JUSTIFIED;
    Y = 400;
    W = 75;
    H = 35;

    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int) nes.ppu.r.controller;;
    const char *controllerVal = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, controllerVal, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = Y;
    box.w = W;
    box.h = H;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    X += 25;


    // Mask Register Text & Values
    surface = TTF_RenderText_Solid(styled_font, mText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 450;
    box.w = 175;
    box.h = 35;

    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    X = RIGHT_R_JUSTIFIED;
    Y = 450;
    W = 75;
    H = 35;

    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int) nes.ppu.r.mask;;
    const char *maskVal = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, maskVal, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = X;
    box.y = Y;
    box.w = W;
    box.h = H;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    X += 25;



    // Status Register
    surface = TTF_RenderText_Solid(styled_font, sText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 500;
    box.w = 175;
    box.h = 35;

    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    
    X = RIGHT_R_JUSTIFIED;
    Y = 500;
    W = 75;
    H = 35;

    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int) nes.ppu.r.status;;
    const char *statusVal = text_ss.str().c_str();
    surface = TTF_RenderText_Solid(lg_font, statusVal, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = X;
    box.y = Y;
    box.w = W;
    box.h = H;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


inline void MARS::drawSystemComponents()
{
    int global_count = nes.NES_SystemClock;

    // Draw MARS Title
    //std::string s_marsTitle = "M.A.R.S";
    const char *marsTitle = "M.A.R.S";
    surface = TTF_RenderText_Solid(space_font, marsTitle, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = 100;
    box.y = 10;
    box.w = 350;
    box.h = 120;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    //std::string s_marsSubTitle = "Marcus's Arcade System";
    const char *marsSubTitle = "Marcus's ARcade System";
    surface = TTF_RenderText_Solid(styled_font, marsSubTitle, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = 130;
    box.y = 150;
    box.w = 290;
    box.h = 45;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw Selected Console
    const char *consoleText = "CONSOLE: ";
    surface = TTF_RenderText_Solid(styled_font, consoleText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = LEFT_L_JUSTIFIED;
    box.y = 300;
    box.w = 200;
    box.h = 45;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    const char *console = selectedConsole.c_str();
    surface = TTF_RenderText_Solid(styled_font, console, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = LEFT_R_JUSTIFIED;
    box.y = 300;
    box.w = 150;
    box.h = 45;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Draw Selected Game
    const char *gameText = "GAME: ";
    surface = TTF_RenderText_Solid(styled_font, gameText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = LEFT_L_JUSTIFIED;
    box.y = 350;
    box.w = 200;
    box.h = 45;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    const char *game = selectedGame.c_str();
    surface = TTF_RenderText_Solid(styled_font, game, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = LEFT_R_JUSTIFIED;
    box.y = 350;
    box.w = 200;
    box.h = 45;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    Y = 900;
    // Draw Global Counter Text
    //std::string str = "Global Cycle:";
    const char *gCountText = "GLOBAL CYCLE:";
    surface = TTF_RenderText_Solid(styled_font, gCountText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 950;
    box.w = 125;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw Global Counter Value
    std::string str2 = to_string(global_count);
    const char *gCountVal = str2.c_str();
    surface = TTF_RenderText_Solid(lg_font, gCountVal, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 950;
    box.w = 150;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Draw Instruction Text
    //std::string str3 = "Instruction:";
    const char *instructText = "INSTRUCTION:";
    surface = TTF_RenderText_Solid(styled_font, instructText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 1000;
    box.w = 150;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw Instruction Value
    std::string instruction = nes.cpu.d.get_instruction;
    const char *instructVal = instruction.c_str();
    surface = TTF_RenderText_Solid(lg_font, instructVal, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 1000;
    box.w = 100;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Draw Address Mode Text
    //std::string str4 = "Addr Mode:";
    const char *addrmodeText = "ADDR MODE:";
    surface = TTF_RenderText_Solid(styled_font, addrmodeText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 1050;
    box.w = 150;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw Address Mode Value
    std::string addrm = nes.cpu.d.get_addrmode;
    const char *addrmode = addrm.c_str();
    surface = TTF_RenderText_Solid(lg_font, addrmode, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 1050;
    box.w = 100;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Draw Opcodes Text
    //std::string str5 = "Opcodes:";
    const char *optext = "OPCODES:";
    surface = TTF_RenderText_Solid(styled_font, optext, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 1100;
    box.w = 125;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw Opcodes Value
    text_ss.str( std::string() );
    text_ss.clear();
    text_ss << std::hex << (int)nes.cpu.d.get_opcode;
    text_ss << " " << nes.cpu.d.get_opcode_instruction[0];
    text_ss << " " << nes.cpu.d.get_opcode_instruction[1];
    const char *opcode = text_ss.str().c_str();
    
    surface = TTF_RenderText_Solid(lg_font, opcode, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED;
    box.y = 1100;
    box.w = 200;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


    // Draw Disassembly Log Title
    //const char *logTitle = "- Disassembly -";
    surface = TTF_RenderText_Solid(styled_font, "- DISASSEMBLY -", Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = 150;
    box.y = 475;
    box.w = 250;
    box.h = 30;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    
    // Draw System Disassembly
    auto vDisassembly = nes.cpu.d.disassembly;
    int j = 0;
    X = LEFT_L_JUSTIFIED;
    Y = 525;
    W = 425;
    H = 35;

    for(auto i = 0; i != vDisassembly.size(); i++)
    {
        const char* disassembly = vDisassembly[i].c_str();

        surface = TTF_RenderText_Solid(sm_font, disassembly, White);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        box.x = X;
        box.y = Y;
        box.w = W;
        box.h = H;
        SDL_RenderCopy(renderer, texture, NULL, &box);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        Y += 35;
        j++;

    }; 
    
    // Draw FPS Text
    const char *fpsText = "FPS";
    surface = TTF_RenderText_Solid(styled_font, fpsText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 250;
    box.w = 90;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    

    // Draw Framerate
    std::string sFps = std::to_string(FPS);
    const char *fpsVal = sFps.c_str();
    surface = TTF_RenderText_Solid(lg_font, fpsVal, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED - 50;
    box.y = 250;
    box.w = 75;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw Nes Clock Speed Text
    const char *csText = "CLOCK SPEED";
    surface = TTF_RenderText_Solid(styled_font, csText, Red);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_L_JUSTIFIED;
    box.y = 300;
    box.w = 120;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    

    // Draw Nes Clock Speed 
    marsSpeed = SDL_GetTicks();
    marsSpeed = (marsSpeed - marsStart);
    marsStart = SDL_GetTicks();
    sFps = std::to_string((marsSpeed));
    const char *mars_clock = sFps.c_str();
    surface = TTF_RenderText_Solid(lg_font, mars_clock, White);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    box.x = RIGHT_R_JUSTIFIED - 50;
    box.y = 300;
    box.w = 75;
    box.h = 35;
    SDL_RenderCopy(renderer, texture, NULL, &box);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}


void MARS::drawPixels()
{

    if (nes.ppu.frameComplete == true ) // || debMode == true
    {
        //pixelTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, LOGICAL_WIDTH, LOGICAL_HEIGHT);

        SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
        int32_t pitch = 0;

        // This will hold a pointer to the memory position in VRAM where our Back Buffer texture lies
        uint32_t* pixelBuffer = nullptr;

        // Lock the memory in order to write our Back Buffer image to it
        if (!SDL_LockTexture(pixelTexture, NULL, (void**)&pixelBuffer, &pitch))
        {
            // The pitch of the Back Buffer texture in VRAM must be divided by four bytes
            // as it will always be a multiple of four
            pitch /= sizeof(uint32_t);

            // Fill texture with  pixels
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
            //SDL_DestroyTexture(pixelTexture);

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

    // Lock the memory in order to write our Back Buffer image to it
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
        pat.box.y = 0;
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
        pat.box.y = 0;
        pat.box.w = 128;
        pat.box.h = 128;
        SDL_UnlockTexture(pat.texture);
        SDL_RenderCopy(renderer, pat.texture, NULL, &pat.box);
        SDL_DestroyTexture(pat.texture);
    };

}

void MARS::resetPatternTables(uint8_t palette)
{
    //palette &= 0x03;
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
    Y = 180;
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
            //Y += 40;
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

            //wLog << "\t R/W:";  wLog << nes.bus_accessType;
            //wLog << "\t\t Controller:";  wLog << std::hex << (int) nes.controllers[0];
            //wLog << "\t\t Controller State:";  wLog << std::hex << (int) nes.controllerState[0];


            //wLog << "\t\t Bus Data:";  wLog << std::hex << (int) nes.bus_data;

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
        //drawRam();

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

bool MARS::debugMode()
{
    debMode = true;

    while (SDL_WaitEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                SDL_Quit();
                return false;

            case SDL_KEYDOWN:
                keyboardInput(event);
                break;
                
            default:
                break;
        };
    };
    return true;
}

void MARS::init()
{
    marsStart = SDL_GetTicks();

    // Initialize SDL 
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    // Main window & Main Renderer
    window = SDL_CreateWindow("MARS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);;
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

    //mars.debugMode();       // Debugger Event Handler
    mars.eventHandler();    // Main Event Handler
    SDL_Quit();

    return 0;
}









//nes.cpuRam[0xFFFC] = 0x00;
//nes.cpuRam[0xFFFD] = 0xC0;

// COMPILE
//g++ *.cpp -IC:\SDL_32bit\i686-w64-mingw32\include\SDL2 -IC:\SDL_ttf\include\SDL2 -LC:\SDL_32bit\i686-w64-mingw32\lib -LC:\SDL_ttf\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o mars.exe

// g++ *.cpp -IC:\SDL_32bit\i686-w64-mingw32\include\SDL2 -IC:\SDL_ttf\include\SDL2 -LC:\SDL_32bit\i686-w64-mingw32\lib -LC:\SDL_ttf\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o mars.exe