#pragma once
#include "Definitions.h"
#include <string>

struct Cartridge {


	Cartridge(std::string name);
	~Cartridge();

	byte operator[](unsigned int address);

	std::string title;
	MemoryBankController mbc;
	byte* rom;
	byte* ram;
	bool containsRam;
	bool battery;
	bool timer;
	int romSize;
	int ramSize;

	bool ramEnable;
	bool mode;
	byte bankNum;

private:

	void setMBC();
	void setRomSize();
	void setRamSize();
	void setTitle();

};