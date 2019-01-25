#include "CPU.h"

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
		
		if (mem.read(0xFF44) > 153) {
			mem.memory[0xFF44] = 0;
		}

		byte currentLine = mem.read(0xFF44);

		scanlineCounter += 456;


		if (currentLine == 144) {
			requestInterrupt(0x01);
		}
		else if (currentLine < 144) {
			drawScanline();
		}
		
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
			SDL_SetRenderDrawColor(renderer, screen[i][j][0], screen[i][j][1], screen[i][j][2], 255);
			SDL_RenderDrawPoint(renderer, i, j);
		}
	}

	SDL_RenderPresent(renderer);

}
