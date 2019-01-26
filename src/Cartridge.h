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
	bool ram;
	bool battery;
	bool timer;
	int romSize;
	int ramSize;

private:

	//void openRom();
	void setMBC();
	void setRomSize();
	void setRamSize();
	void setTitle();

};