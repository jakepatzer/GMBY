#pragma once
#include "Definitions.h"
#include "Cartridge.h"

class CPU;

enum Register {

	DIV = 0xFF04,
	TIMA = 0xFF05,
	TMA = 0xFF06,
	TAC = 0xFF07,
	IE = 0xFFFF,
	IF = 0xFF0F,
	LCD_CONTROL = 0xFF40,
	SCROLL_Y = 0xFF42,
	SCROLL_X = 0xFF43,
	WINDOW_Y = 0xFF4A,
	WINDOW_X = 0xFF4B

};

class MMU
{
public:
	MMU(CPU* cpu);
	~MMU();

	void write(word address, byte data);
	byte read(word address);
	word read16(word address);
	void DMA_Transfer(byte data);

	Cartridge* cartridge;
	byte* memory;
	//byte* cartridge;
	CPU* cpu;
	
};

/*
0000-3FFF 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
4000-7FFF 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
8000-9FFF 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
A000-BFFF 8KB External RAM (in cartridge, switchable bank, if any)
C000-CFFF 4KB Work RAM Bank 0 (WRAM)
D000-DFFF 4KB Work RAM Bank 1 (WRAM) (switchable bank 1-7 in CGB Mode)
E000-FDFF Same as C000-DDFF (ECHO) (typically not used)
FE00-FE9F Sprite Attribute Table (OAM)
FEA0-FEFF Not Usable
FF00-FF7F I/O Ports
FF80-FFFE High RAM (HRAM)
FFFF Interrupt Enable Register
*/
