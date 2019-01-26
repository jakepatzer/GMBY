#include "CPU.h"
#include<stdio.h>
#include<iostream>
#include<fstream>

using std::cout;

CPU::CPU(SDL_Window* w, SDL_Renderer* r)
{
	loadGame();
	window = w;
	renderer = r;
	reg.af = 0x01B0;
	reg.bc = 0x0013;
	reg.de = 0x00D8;
	reg.hl = 0x014D;
	reg.pc = 0x100;
	reg.sp = 0xFFFE;
	timerCounter = 1024;
	dividerCounter = 0;
	scanlineCounter = 456;
	halt = false;
	stop = false;
	IME = true;
	currentCycles = 0;

	for (int i = 0; i < 160; i++) {
		for (int j = 0; j < 144; j++) {
			for (int k = 0; k < 3; k++) {
				screen[i][j][k] = 0;
			}
		}
	}

	totalCycles = 23440332;
	lineNum = 1;
}


CPU::~CPU()
{
}

void CPU::updateTimers(byte cycles)
{
	//update the divider register
	dividerCounter += cycles;
	if (dividerCounter > 255) {
		dividerCounter = dividerCounter - 256;
		mem.memory[DIV]++;
	}

	//check if clock is enabled
	if (bitSelect(mem.read(TAC), 2)) {

		timerCounter -= cycles;

		if (timerCounter <= 0) {

			word frequency = 0;

			if ((mem.read(TAC) & 0xFC) == 0) {
				frequency = 1024;
			}
			else if ((mem.read(TAC) & 0xFC) == 1) {
				frequency = 16;
			}
			else if ((mem.read(TAC) & 0xFC) == 2) {
				frequency = 64;
			}
			else {
				frequency = 256;
			}

			timerCounter += frequency;

			if (mem.read(TIMA) == 255) {
				mem.write(TIMA, mem.read(TMA));
				requestInterrupt(0x04);
			}
			else {
				mem.write(TIMA, mem.read(TIMA) + 1);
			}

		}

	}

}

byte CPU::interrupts()
{
	byte cycles = 0;
	if (mem.read(IF)) {
		for (byte i = 0; i < 5; i++) {
			if (bitSelect(mem.read(IF), i)) {
				if (bitSelect(mem.read(IE), i)) {
					if (IME) {
						executeInterrupt(i);
						cycles += 20;
					}
				}
			}
		}
	}
	return cycles;
}

void CPU::executeInterrupt(byte interrupt)
{
	IME = false;
	halt = false;
	reg.sp--;
	mem.write(reg.sp, reg.pc >> 8);
	reg.sp--;
	mem.write(reg.sp, reg.pc & 0xFF);

	switch (interrupt) {

	case 0:
		mem.write(IF, mem.read(IF) & ~(0x01));
		reg.pc = 0x40;
		break;
	case 1:
		mem.write(IF, mem.read(IF) & ~(0x02));
		reg.pc = 0x48;
		break;
	case 2:
		mem.write(IF, mem.read(IF) & ~(0x04));
		reg.pc = 0x50;
		break;
	case 3:
		mem.write(IF, mem.read(IF) & ~(0x08));
		reg.pc = 0x60;
		break;
	}
	
}

void CPU::update()
{
	
	while (currentCycles < MAX_CYCLES_PER_REFRESH) {

		updateJoypad();
		byte cycles = interrupts();

		byte opcode = mem.read(reg.pc);
		reg.pc++;
		cycles += execute(opcode);
			
		updateTimers(cycles);
		updateScreen(cycles);
		currentCycles += cycles;
		totalCycles += cycles;

		lineNum++;
			
		if (mem.read(0xFF01) == 0xFF) {
			//mem.memory[0xFF01] = 0x00;
			//debug = true;
			//printCurrentState(opcode);
			//system("PAUSE");
		}

	}
	renderScreen();
	currentCycles -= MAX_CYCLES_PER_REFRESH;
}

void CPU::run()
{
	while (true) {
		update();
	}
}

void CPU::loadGame()
{
	memset(mem.cartridge, 0, sizeof(mem.cartridge));
	FILE* rom;
	rom = fopen("roms//Tetris.gb", "rb");
	fread(mem.cartridge, 1, 0x200000, rom);
	fclose(rom);

	for (int i = 0; i < 0x8000; i++) {
		mem.memory[i] = mem.cartridge[i];
	}
}

void CPU::printCurrentState(byte opcode)
{

	cout << "l:" << std::dec << lineNum << " pc:" << std::hex << reg.pc << " af:" << reg.af << " bc:" << reg.bc << " de:" << reg.de
		<< " hl:" << reg.hl << " sp:" << reg.sp << " ly:" << (int)mem.read(0xFF44) << " cy:" << std::dec << totalCycles << std::endl;
	
}
