# Marcus's Arcade System - NES Emulator![nes_sample](https://user-images.githubusercontent.com/72711596/139308673-edbacb3c-69ec-44eb-8c5d-4d147261673d.png)

# A simple NES emulator prototype using SDL - Still in Devlopment*

Huge thank you to the following people for making this journey possible:
   - OneLoneCoder: Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
   - NESDev: https://www.nesdev.com/
   - EmuDev: https://www.reddit.com/r/EmuDev/

# Bugs to Fix / To-Do List:
   - PPU 
		1. Improper sprite color palettes on SMB (Likely mirroring bug)
		2. Fix Sprite Priorty issue on SMB
		3. re-write DMA transfers to better fit NES's logic 
          
   - MAPPERS 
		1. Implement Mapper 002
		2. Implement Mapper 004
		3. Implement Mapper001

   - MARS (SDL)
		1. Design elegant way to switch between ROMs in runtime
		2. Higher accuracy in acheiving a flat rate of 60 FPS
		3. Resolution Scaling for Rendering
		4. Implement Save States

   - CPU: 
		1. Re-design address modes and instructions for cycle accuaracy
 
   - CARTRIDGE
		1. Re-evaluate logic for PPU mapping, cartridge overrides
		2. Implement INES2.0 file support
   - APU
   		1. Implement Sound APU unit
