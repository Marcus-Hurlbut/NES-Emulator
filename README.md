# Marcus's Arcade System - NES Emulator
![nes_sample_picture](https://user-images.githubusercontent.com/72711596/140624067-b8f22473-2f48-4f06-83ac-9f06b4b8a3e2.png)


# A simple NES emulator using SDL in C++ - Still in Development*

Huge thank you to the following people for making this journey possible:
   - OneLoneCoder: Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
   - NESDev: https://www.nesdev.com/
   - EmuDev: https://www.reddit.com/r/EmuDev/

# Dependencies
   - MinGW32 g++
   - SDL library
   - TTL library

# Bugs to Fix / To-Do List:          
   - MAPPERS
		1. Implement Mapper 004
		2. Implement Mapper 003
		3. Implement Mapper 001

   - MARS (SDL)
		1. Design elegant way to switch between ROMs in runtime
		2. Higher accuracy in acheiving a flat rate of 60 FPS [CRITICAL]
		4. Implement Save States

   - CPU: 
		1. Redesign address modes and instructions to achieve cycle accuaracy
		2. Optimize Disassembly Log & add to separate module 
 

   - APU
   		1. Implement Sound APU unit
