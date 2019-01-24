#pragma once
#include "Definitions.h"

enum Flag {

	ZFLAG = 0x80,
	NFLAG = 0x40,
	HFLAG = 0x20,
	CFLAG = 0x10

};

struct Registers {

	union {
		struct {
			byte f;
			byte a;
		};
		word af;
	};

	union {
		struct {
			byte c;
			byte b;
		};
		word bc;
	};

	union {
		struct {
			byte e;
			byte d;
		};
		word de;
	};

	union {
		struct {
			byte l;
			byte h;
		};
		word hl;
	};

	word sp;
	word pc;

	bool getFlag(byte flag);
	void setFlag(byte flag);
	void resetFlag(byte flag);
	void toggleFlag(byte flag);

};


