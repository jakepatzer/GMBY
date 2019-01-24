#include "MMU.h"
#include<iostream>
#include "CPU.h"


MMU::MMU()
{
	//this->cpu = cpu;
	memory = new byte[0x10000];
	cartridge = new byte[0x200000];

	for (int i = 0; i < 0xFFFF; i++) {
		memory[i] = 0;
	}
	memory[0xFF05] = 0x00;
	memory[0xFF06] = 0x00;
	memory[0xFF07] = 0x00;
	memory[0xFF10] = 0x80;
	memory[0xFF11] = 0xBF;
	memory[0xFF12] = 0xF3;
	memory[0xFF14] = 0xBF;
	memory[0xFF16] = 0x3F;
	memory[0xFF17] = 0x00;
	memory[0xFF19] = 0xBF;
	memory[0xFF1A] = 0x7F;
	memory[0xFF1B] = 0xFF;
	memory[0xFF1C] = 0x9F;
	memory[0xFF1E] = 0xBF;
	memory[0xFF20] = 0xFF;
	memory[0xFF21] = 0x00;
	memory[0xFF22] = 0x00;
	memory[0xFF23] = 0xBF;
	memory[0xFF24] = 0x77;
	memory[0xFF25] = 0xF3;
	memory[0xFF26] = 0xF1;
	memory[0xFF40] = 0x91;
	memory[0xFF42] = 0x00;
	memory[0xFF43] = 0x00;
	memory[0xFF45] = 0x00;
	memory[0xFF47] = 0xFC;
	memory[0xFF48] = 0xFF;
	memory[0xFF49] = 0xFF;
	memory[0xFF4A] = 0x00;
	memory[0xFF4B] = 0x00;
	memory[0xFFFF] = 0x00;


	//memory[0xFF44] = 0x91;
	/////
	memory[0xFF00] = 0x1F;
}


MMU::~MMU()
{
	delete[] memory;
	delete[] cartridge;
}

void MMU::write(word address, byte data)
{

	//Prevent writes to ROM
	if (address < 0x8000) {
	}

	else if ((address >= 0xC000) && (address <= 0xDFFF)) {
		memory[address] = data;
		memory[address + 0x2000] = data;
	}

	//Write to RAM and ECHO RAM
	else if (address >= 0xE000 && address < 0xFE00) {

		memory[address] = data;
		memory[address - 0x2000] = data;

	}

	//Prevent writing to unusable memory
	else if (address >= 0xFEA0 && address < 0xFEFF) {
	}
	else if (address == 0xFF01) {
		std::cout << data;
	}
	else if (address == 0xFF02 && data == 0x81) {
		//std::cout << data << std::endl;
	}

	else if (address == 0xFF04) {
		memory[address] = 0;
		//reset dividercounter
	}
	else if (address == 0xFF44) {
		memory[address] = 0;
	} 
	else if (address == 0xFF46) {
		DMA_Transfer(data);
	}
	else {
		memory[address] = data;
	}
}

void MMU::write16(word address, word data)
{
	byte hi = (data & 0xFF00) >> 8;
	byte lo = data & 0x00FF;
	write(address, hi);
	write(address + 1, lo);
}

byte MMU::read(word address) {
	if (address == 0xFF00) {
		return 0x1F;
	}

	//Interrupt Flags; bits 5-7 always return 1
	else if (address == 0xFF0F) {
		return memory[0xFF0F] | 0xE0;
	}

	else {
		return memory[address];
	}

}

word MMU::read16(word address)
{
	return (read(address+1) << 8) | read(address);
}

void MMU::DMA_Transfer(byte data)
{

	word address = (data << 8);

	for (int i = 0; i < 0xA0; i++) {
		memory[0xFE00 + i] = read(address + i);
	}

}
