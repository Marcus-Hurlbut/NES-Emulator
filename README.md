# Marcus's Arcade System - NES Emulator
![nes_sample_picture](https://user-images.githubusercontent.com/72711596/140624067-b8f22473-2f48-4f06-83ac-9f06b4b8a3e2.png)


# A simple NES emulator using SDL in C++ - Still in Development*

Huge thank you to the following people for making this journey possible:
   - OneLoneCoder: Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
   - NESDev: https://www.nesdev.com/
   - EmuDev: https://www.reddit.com/r/EmuDev/

# Bugs to Fix / To-Do List:
   - PPU 
		1. re-write DMA transfers to better fit NES's logic
          
   - MAPPERS 
		1. Implement Mapper 002
		2. Implement Mapper 004
		3. Implement Mapper 001

   - MARS (SDL)
		1. Design elegant way to switch between ROMs in runtime
		2. Higher accuracy in acheiving a flat rate of 60 FPS [CRITICAL]
		3. Resolution Scaling for Rendering
		4. Implement Save States

   - CPU: 
		1. Re-design address modes and instructions to achieve cycle accuaracy
 
   - CARTRIDGE
		1. Re-evaluate logic for PPU mapping, cartridge overrides
		2. Implement INES2.0 file support [CRITICAL]
   - APU
   		1. Implement Sound APU unit
