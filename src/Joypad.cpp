#include "CPU.h"
#include <windows.h>

void CPU::updateJoypad() {

	byte joypadState = 0x0F;
	bool interrupt = false;

	//D-Pad
	if (!bitSelect(mem.read(0xFF00), 4)) {

		//D-pad left (left arrow key)
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			joypadState = bitReset(joypadState, 1);
		}

		//D-pad right (right arrow key)
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			joypadState = bitReset(joypadState, 0);
		}

		//D-pad down (down arrow key)
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			joypadState = bitReset(joypadState, 3);
		}

		//D-pad up (up arrow key)
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			joypadState = bitReset(joypadState, 2);
		}

	}

	//Buttons
	else if (!bitSelect(mem.read(0xFF00), 5)) {

		//A (Z key)
		if (GetAsyncKeyState(0x5A) & 0x8000) {
			joypadState = bitReset(joypadState, 0);
		}

		//B (X key)
		if (GetAsyncKeyState(0x58) & 0x8000) {
			joypadState = bitReset(joypadState, 1);
		}

		//Start (Enter)
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			joypadState = bitReset(joypadState, 3);
		}

		//Select (Shift)
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
			joypadState = bitReset(joypadState, 2);
		}

	}

	for (int i = 0; i < 4; i++) {
		if (bitSelect(mem.read(0xFF00), i) && !bitSelect(joypadState, i)) {
			interrupt = true;
		}
	}

	mem.memory[0xFF00] &= 0xF0;
	mem.memory[0xFF00] |= joypadState;

	if (interrupt) requestInterrupt(0x10);

}
