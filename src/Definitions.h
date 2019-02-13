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

enum MemoryBankController {

	NONE,
	MBC1,
	MBC2,
	MBC3

};
