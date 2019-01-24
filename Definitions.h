#pragma once
#include<cstdint>

typedef uint8_t byte;
typedef uint16_t word;
typedef int8_t signed_byte;
typedef int16_t signed_word;

const int FRAMERATE = 60;
const int CLOCKSPEED = 4194304;
const int MAX_CYCLES_PER_REFRESH = CLOCKSPEED / FRAMERATE;

enum Color {

	WHITE,
	LIGHT_GREY,
	DARK_GREY,
	BLACK

};


/*

word dividerOld = mem.dividerRegister;
	mem.dividerRegister += cycles;

	byte timerINC = 0;

	if (bitSelect(mem.read(TAC), 2)) {
		if (mem.read(TAC) & 0xFC == 0) {
			if (bitSelect(dividerOld, 9) == 1 && bitSelect(mem.dividerRegister, 9) == 0) {
				timerINC = 1;
			}
		}
		else if (mem.read(TAC) & 0xFC == 1) {
			if (bitSelect(dividerOld, 3) == 1 && bitSelect(mem.dividerRegister, 3) == 0) {
				timerINC = 1;
			}
		}
		else if (mem.read(TAC) & 0xFC == 2) {
			if (bitSelect(dividerOld, 5) == 1 && bitSelect(mem.dividerRegister, 5) == 0) {
				timerINC = 1;
			}
		}
		else {
			if (bitSelect(dividerOld, 7) == 1 && bitSelect(mem.dividerRegister, 7) == 0) {
				timerINC = 1;
			}
		}
	}
*/