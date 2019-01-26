#pragma once
#include "Registers.h"
#include "Definitions.h"
#include "MMU.h"
#include<SDL.h>
#include<string>

class CPU
{
public:
	CPU(SDL_Window* w, SDL_Renderer* r);
	~CPU();

	void updateTimers(byte cycles);
	byte interrupts();
	void executeInterrupt(byte interrupt);
	void update();
	void run();
	void loadGame();
	void printCurrentState(byte opcode);

	//////Joypad.cpp//////
	void updateJoypad();

	//////Opcodes.cpp//////
	byte execute(byte opcode);
	byte executeCB(byte opcode);

	//////Graphics.cpp//////
	void updateScreen(byte cycles);
	bool LCD_Enabled();
	void setLCDStatus();
	void drawScanline();
	void renderTiles();
	void renderSprites();
	Color getColor(byte colorNum, word address);
	void renderScreen();

	//////Operations.cpp//////
	void ADD_8bit(byte value);
	void ADD_16bit(word& dest, word value);
	void ADD_sp(signed_byte value);
	void ADC(byte value);
	void CP(byte value);
	void SUB(byte value);
	void SBC(byte value);
	void AND(byte value);
	void XOR(byte value);
	void OR(byte value);
	byte INC_8bit(byte value);
	byte DEC_8bit(byte value);
	void DAA();
	void RLCA();
	byte RLC(byte value);
	void RLA();
	byte RL(byte value);
	void RRCA();
	byte RRC(byte value);
	void RRA();
	byte RR(byte value);
	byte SLA(byte value);
	byte SRA(byte value);
	byte SRL(byte value);
	byte SWAP(byte value);
	void BIT(byte value, byte mask);
	void JP(byte addressL, byte addressH);
	void JR(signed_byte offset);
	void RST(byte low);
	void LDHL(signed_byte e);
	void CALL(byte valueL, byte valueH);
	void RET();
	void RETI();
	void requestInterrupt(byte bit);
	byte bitSelect(byte value, byte bit);
	byte bitSet(byte value, byte bit);
	byte bitReset(byte value, byte bit);



	long int lineNum;

private:
	Registers reg;
	MMU mem = MMU(this);
	int screen[160][144][3];
	int timerCounter;
	int dividerCounter;
	int scanlineCounter;
	bool halt;
	bool stop;
	byte IME;
	SDL_Window* window;
	SDL_Renderer* renderer;
	unsigned int currentCycles;

	bool debug = false;
	int totalCycles;
};
