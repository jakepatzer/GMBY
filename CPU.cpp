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
	reg.af = 0x0190; //0x01B0;
	reg.bc = 0x0013; //0x0013;
	reg.de = 0x00d8; //0x00D8;
	reg.hl = 0x014d; //0x014D;
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

	if (mem.read(IF)) {
		for (byte i = 0; i < 5; i++) {
			if (bitSelect(mem.read(IF), i)) {
				if (bitSelect(mem.read(IE), i)) {
					//halt = false;
					if (IME) {
						executeInterrupt(i);
						return 20;
					}
				}
			}
		}
	}
	return 0;
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

void CPU::updateScreen(byte cycles)
{

	setLCDStatus();

	if (LCD_Enabled()) {
		scanlineCounter -= cycles;
	}
	else {
		return;
	}


	if (scanlineCounter <= 0) {
		mem.memory[0xFF44]++;
		
		if (mem.read(0xFF44) > 153) { //0x99
			mem.memory[0xFF44] = 0;
		}

		byte currentLine = mem.read(0xFF44);

		scanlineCounter += 456;


		if (currentLine == 144) { //0x90
			requestInterrupt(0x01);
		}
		else if (currentLine < 144) {
			drawScanline();
		}

		//mem.memory[0xFF44]++;

		
	}

}

bool CPU::LCD_Enabled()
{
	return bitSelect(mem.read(0xFF40), 7);
}

void CPU::setLCDStatus()
{

	byte status = mem.read(0xFF41);
	if (!LCD_Enabled()) {

		scanlineCounter = 456;
		mem.memory[0xFF44] = 0;
		status &= 0xFC;
		status |= 1;
		mem.write(0xFF41, status);
		return;

	}

	byte currentLine = mem.read(0xFF44);
	byte currentMode = status & 0x3;

	byte mode = 0;
	bool interrupt = false;

	if (currentLine >= 144) {
		mode = 1;
		status = bitSet(status, 0);
		status = bitReset(status, 1);
		interrupt = bitSelect(status, 4);
	}
	else {

		if (scanlineCounter >= 376) {
			mode = 2;
			status = bitSet(status, 1);
			status = bitReset(status, 0);
			interrupt = bitSelect(status, 5);
		}
		else if (scanlineCounter >= 204) {
			mode = 3;
			status = bitSet(status, 1);
			status = bitSet(status, 0);
		}
		else {
			mode = 0;
			status = bitReset(status, 1);
			status = bitReset(status, 0);
			interrupt = bitSelect(status, 3);
		}

	}
	
	if (interrupt && (mode != currentMode)) {
		requestInterrupt(0x02);
	}

	if (currentLine == mem.read(0xFF45)) {
		status = bitSet(status, 2);
		if (bitSelect(status, 6)) {
			requestInterrupt(0x02);
		}
	}
	else {
		status = bitReset(status, 2);
	}
	mem.write(0xFF41, status);
}

void CPU::drawScanline()
{
	byte control = mem.read(0xFF40);
	if (bitSelect(control, 0)) {
		renderTiles();
	}
	if (bitSelect(control, 1)) {
		renderSprites();
	}
}

void CPU::renderTiles()
{

	word tileData = 0;
	word backgroundMemory = 0;
	bool unsig = true;

	byte scrollY = mem.read(SCROLL_Y);
	byte scrollX = mem.read(SCROLL_X);
	byte windowX = mem.read(WINDOW_X) - 7;
	byte windowY = mem.read(WINDOW_Y);

	bool window = false;

	if (bitSelect(mem.read(LCD_CONTROL), 5)) {
		if (windowY <= mem.read(0xFF44)) {
			window = true;
		}
	}

	if (bitSelect(mem.read(LCD_CONTROL), 4)) {
		tileData = 0x8000;
	}
	else {
		tileData = 0x8800;
		unsig = false;
	}

	if (!window) {
		if (bitSelect(mem.read(LCD_CONTROL), 3)) {
			backgroundMemory = 0x9C00;
		}
		else {
			backgroundMemory = 0x9800;
		}
	}
	else {
		if (bitSelect(mem.read(LCD_CONTROL), 6)) {
			backgroundMemory = 0x9C00;
		}
		else {
			backgroundMemory = 0x9800;
		}
	}

	byte yPosition = 0;

	if (!window) {
		yPosition = scrollY + mem.read(0xFF44);
	}
	else {
		yPosition = mem.read(0xFF44) - windowY;
	}

	word tileRow = ((byte)(yPosition / 8)) * 32;

	for (byte pixel = 0; pixel < 160; pixel++) {
		byte xPosition = pixel + scrollX;

		if (window && pixel >= windowX) {
			xPosition = pixel - windowX;
		}

		word tileCol = xPosition / 8;
		signed_word tileNum;

		word tileAddress = backgroundMemory + tileRow + tileCol;

		if (unsig) {
			tileNum = (byte)mem.read(tileAddress);
		}
		else {
			tileNum = (signed_byte)mem.read(tileAddress);
		}

		word tileLocation = tileData;

		if (unsig) {
			tileLocation += tileNum * 16;
		}
		else {
			tileLocation += (tileNum + 128) * 16;
		}

		byte line = (yPosition % 8) * 2;
		byte data1 = mem.read(tileLocation + line);
		byte data2 = mem.read(tileLocation + line + 1);

		int colorBit = ((xPosition % 8) - 7) * (-1);

		int colorNum = bitSelect(data2, colorBit);
		colorNum <<= 1;
		colorNum |= bitSelect(data1, colorBit);

		Color color = getColor(colorNum, 0xFF47);
		int red = 0;
		int green = 0;
		int blue = 0;

		switch (color) {
		case WHITE:
			red = 255;
			green = 255;
			blue = 255;
			break;
		case LIGHT_GREY:
			red = 0xCC;
			green = 0xCC;
			blue = 0xCC;
			break;
		case DARK_GREY:
			red = 0x77;
			green = 0x77;
			blue = 0x77;
			break;
		}

		int check = mem.read(0xFF44);

		if ((check < 0) || (check > 143) || (pixel < 0) || (pixel > 159)) {
			continue;
		}

		screen[pixel][check][0] = red;
		screen[pixel][check][1] = green;
		screen[pixel][check][2] = blue;

	}

}

void CPU::renderSprites()
{

	bool use8x16 = false;
	if (bitSelect(mem.read(LCD_CONTROL), 2)) {
		use8x16 = true;
	}

	for (int sprite = 0; sprite < 40; sprite++) {

		byte index = sprite * 4;
		byte yPosition = mem.read(0xFE00 + index) - 16;
		byte xPosition = mem.read(0xFE00 + index + 1) - 8;
		byte tileLocation = mem.read(0xFE00 + index + 2);
		byte attributes = mem.read(0xFE00 + index + 3);

		bool yFlip = bitSelect(attributes, 6);
		bool xFlip = bitSelect(attributes, 5);

		int scanline = mem.read(0xFF44);

		int ySize = 8;

		if (use8x16) {
			ySize = 16;
		}

		if ((scanline >= yPosition) && (scanline < (yPosition + ySize))) {
			int line = scanline - yPosition;

			if (yFlip) {
				line -= ySize;
				line *= -1;
			}

			line *= 2;
			word dataAddress = (0x8000 + (tileLocation * 16)) + line;
			byte data1 = mem.read(dataAddress);
			byte data2 = mem.read(dataAddress + 1);

			for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
				int colorBit = tilePixel;

				if (xFlip) {
					colorBit -= 7;
					colorBit *= -1;
				}

				int colorNum = bitSelect(data2, colorBit);
				colorNum <<= 1;
				colorNum |= bitSelect(data1, colorBit);

				word colorAddress = 0xFF48;
				if (bitSelect(attributes, 4)) {
					colorAddress = 0xFF49;
				}
				
				Color color = getColor(colorNum, colorAddress);

				if (color == WHITE) {
					continue;
				}

				int red = 0;
				int green = 0;
				int blue = 0;

				switch (color) {
				case WHITE:
					red = 255;
					green = 255;
					blue = 255;
					break;
				case LIGHT_GREY:
					red = 0xCC;
					green = 0xCC;
					blue = 0xCC;
					break;
				case DARK_GREY:
					red = 0x77;
					green = 0x77;
					blue = 0x77;
					break;
				}

				int xPix = 0 - tilePixel;
				xPix += 7;

				int pixel = xPosition + xPix;

				if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159)) {
					continue;
				}

				screen[pixel][scanline][0] = red;
				screen[pixel][scanline][1] = green;
				screen[pixel][scanline][2] = blue;

			}

		}

	}

}

Color CPU::getColor(byte colorNum, word address)
{
	Color res = WHITE;
	byte palette = mem.read(address);
	byte hi = 0;
	byte lo = 0;

	switch (colorNum) {
	case 0:
		hi = 1;
		lo = 0;
		break;
	case 1:
		hi = 3;
		lo = 2;
		break;
	case 2:
		hi = 5;
		lo = 4;
		break;
	case 3:
		hi = 7;
		lo = 6;
		break;
	}

	byte color = 0;
	color = bitSelect(palette, hi) << 1;
	color |= bitSelect(palette, lo);

	switch (color) {
	case 0:
		res = WHITE;
		break;
	case 1:
		res = LIGHT_GREY;
		break;
	case 2:
		res = DARK_GREY;
		break;
	case 3:
		res = BLACK;
		break;
	}
	return res;
}

void CPU::renderScreen()
{

	SDL_RenderClear(renderer);

	for (int i = 0; i < 160; i++) {
		
		for (int j = 0; j < 144; j++) {
			//cout << screen[i][j][0] << std::endl;
			SDL_SetRenderDrawColor(renderer, screen[i][j][0], screen[i][j][1], screen[i][j][2], 255);
			SDL_RenderDrawPoint(renderer, i, j);

		}
	}

	SDL_RenderPresent(renderer);

}


void CPU::update()
{
	
	while (currentCycles < MAX_CYCLES_PER_REFRESH) {
		//if (!halt) {

		byte cycles = interrupts();

		byte opcode = mem.read(reg.pc);
		reg.pc++;
		cycles += execute(opcode);
			
		updateTimers(cycles);
		updateScreen(cycles);
		currentCycles += cycles;
		totalCycles += cycles;

		lineNum++;
			
		if (lineNum == 16960 || debug == true) {
			//	debug = true;
			//	printCurrentState(opcode);
			//	system("PAUSE");
		}
				//system("PAUSE");
			
	//	}
	//	else {
	//		updateTimers(4);
	//		updateScreen(4);
	//		currentCycles += 4;
	//	}
		

	}
	renderScreen();
	currentCycles -= MAX_CYCLES_PER_REFRESH;
}

void CPU::run()
{
	cout << std::hex << reg.pc << std::endl;
	while (true) {
		update();
	}
}

void CPU::loadGame()
{
	memset(mem.cartridge, 0, sizeof(mem.cartridge));
	FILE* rom;
	rom = fopen("Boxxle (U) (V1.1) [!].gb", "rb");
	fread(mem.cartridge, 1, 0x200000, rom);
	fclose(rom);

	for (int i = 0; i < 0x8000; i++) {
		mem.memory[i] = mem.cartridge[i];
	}
}

void CPU::printCurrentState(byte opcode)
{
	cout << std::hex;

	cout << "l:" << std::dec << lineNum << " pc:" << std::hex << reg.pc << " af:" << reg.af << " bc:" << reg.bc << " de:" << reg.de << " hl:" << reg.hl << " ly:" << (int)mem.read(0xFF44) << " cy:" << std::dec << totalCycles << std::endl;
	//cout << std::hex << "pc: " << reg.pc << "; opcode: " << (int)opcode << "\n" << std::endl;
}


