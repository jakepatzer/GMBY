#include "Cartridge.h"

Cartridge::Cartridge(std::string path)
{
	rom = new byte[0x200000];
	ram = false;
	battery = false;
	timer = false;

	memset(rom, 0, sizeof(rom));
	FILE* file;
	file = fopen(path.c_str(), "rb");
	fread(rom, 1, 0x200000, file);
	fclose(file);

	setMBC();
	setRomSize();
	setRamSize();
	
}

Cartridge::~Cartridge()
{
	delete[] rom;
}

byte Cartridge::operator[](unsigned int address)
{
	return rom[address];
}

void Cartridge::setMBC()
{
	byte type = rom[0x147];

	switch (type) {

	case 0x00:
		mbc = NONE;
		break;
	case 0x01:
		mbc = MBC1;
		break;
	case 0x02:
		mbc = MBC1;
		ram = true;
		break;
	case 0x03:
		mbc = MBC1;
		ram = true;
		battery = true;
		break;
	case 0x05:
		mbc = MBC2;
		break;
	case 0x06:
		mbc = MBC2;
		battery = true;
		break;
	case 0x08:
		mbc = NONE;
		ram = true;
		break;
	case 0x09:
		mbc = NONE;
		ram = true;
		battery = true;
		break;
	case 0x0F:
		mbc = MBC3;
		timer = true;
		battery = true;
		break;
	case 0x10:
		mbc = MBC3;
		timer = true;
		battery = true;
		ram = true;
		break;
	case 0x11:
		mbc = MBC3;
		break;
	case 0x12:
		mbc = MBC3;
		ram = true;
		break;
	case 0x13:
		mbc = MBC3;
		ram = true;
		battery = true;
		break;
	}

}

void Cartridge::setRomSize()
{

	byte size = rom[0x148];

	switch (size) {

	case 0x00:
		romSize = 32;
		break;
	case 0x01:
		romSize = 64;
		break;
	case 0x02:
		romSize = 128;
		break;
	case 0x03:
		romSize = 256;
		break;
	case 0x04:
		romSize = 512;
		break;
	case 0x05:
		romSize = 1024;
		break;
	case 0x06:
		romSize = 2048;
		break;
	case 0x07:
		romSize = 4096;
		break;
	case 0x08:
		romSize = 8192;
		break;
	case 0x52:
		romSize = 1152;
		break;
	case 0x53:
		romSize = 1280;
		break;
	case 0x54:
		romSize = 1536;
		break;

	}

}

void Cartridge::setRamSize()
{

	byte size = rom[0x149];

	switch (size) {

	case 0:
		ramSize = 0;
		break;
	case 1:
		ramSize = 2;
		break;
	case 2:
		ramSize = 8;
		break;
	case 3:
		ramSize = 32;
		break;
	case 4:
		ramSize = 128;
		break;
	case 5:
		ramSize = 64;
		break;

	}

}

void Cartridge::setTitle()
{

	title = "";
	for (int i = 0; i < 16; i++) {
		title += (char)rom[0x134 + i];
	}

}
