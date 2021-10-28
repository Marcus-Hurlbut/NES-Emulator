# MARS NES![nes_sample](https://user-images.githubusercontent.com/72711596/139308673-edbacb3c-69ec-44eb-8c5d-4d147261673d.png)

# Marcus's ARcade System - NES Emulator
A simple NES emulator prototype using SDL - Still in Devlopment*

Huge thank you to the following people for making this journey possible:
   - OneLoneCoder: Copyright 2018, 2019, 2020, 2021 OneLoneCoder.com
   - NESDev: https://www.nesdev.com/
   - EmuDev: https://www.reddit.com/r/EmuDev/

# Bugs to Fix / To-Do List:
   - Ppu: Improper sprite color palettes on SMB (Likely mirroring bug)
          Fix Sprite Priorty issue on SMB
          re-write DMA transfers to more fit NES's logic 
   - Mappers: Implement Mapper001, Mapper 002, Mapper 004
   - FPS: Higher accuracy in acheiving a flat rate of 60 FPS
   - Mars (SDL): Design elegant way to switch between ROMs in runtime
   - Bus: Add support for Controller 2
   - Cpu: Re-design address modes and instructions to make cycle accuarate
   - Cartridge: Re-evaluate logic for PPU mapping, cartridge overrides
