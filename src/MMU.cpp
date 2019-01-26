#include "MMU.h"
#include<iostream>
#include "CPU.h"


MMU::MMU(CPU* cpu)
{
	this->cpu = cpu;
	memory = new byte[0x10000];
	cartridge = nullptr;

	for (int i = 0; i < 0xFFFF; i++) {
		memory[i] = 0;
	}

	memory[0xFF00] = 0xFF;
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

}


MMU::~MMU()
{
	delete[] memory;
	delete cartridge;
}

void MMU::write(word address, byte data)
{
	
	if (cartridge->mbc == MBC1) {

		if (address < 0x2000) {

			if (data & 0x0F == 0x0A) {
				cartridge->ramEnable = true;
			}
			else {
				cartridge->ramEnable = false;
			}

		}
		else if (address < 0x4000) {

			cartridge->bankNum &= 0xE0;

			if (data & 0x1F == 0) {
				cartridge->bankNum |= 0x01;
			}
			else {
				cartridge->bankNum |= data & 0x1F;
			}

		}
		else if (address < 0x6000) {

			cartridge->bankNum &= 0x9F;
			cartridge->bankNum |= (data & 0x03) << 5;

		}
		else if (address < 0x8000) {
			if (data == 0) {
				cartridge->mode = false;
			}
			else {
				cartridge->mode = true;
			}
		}
		else if (0xA000 <= address && address < 0xBFFF) {

			//RAM mode
			if (cartridge->mode) {

				cartridge->ram[((address - 0xA000) + (((cartridge->bankNum >> 5) & 0x03) * 0x2000))] = data;

			}
			//ROM mode
			else {

				cartridge->ram[address - 0xA000] = data;

			}

		}

	}

	if (cartridge->mbc == MBC2) {

		if (address < 0x2000) {
			if ((address >> 4) & 1) {
				return;
			}
			else {
				if (data & 0x0F == 0x0A) {
					cartridge->ramEnable = true;
				}
				else {
					cartridge->ramEnable = false;
				}
			}
		}
		else if (address < 0x4000) {
			if ((address >> 4) & 1) {
				cartridge->bankNum = data & 0x0F;
			}
			else {
				return;
			}
		}
		else if (address < 0x8000) {
			return;
		}
		else if (0xA000 <= address && address < 0xA200) {
			cartridge->ram[address - 0xA000] = (data & 0x0F);
		}

	}

	//Prevent writes to ROM
	if (address < 0x8000) {
	}

	else if ((0xC000 <= address) && (address < 0xE000)) {
		memory[address] = data;
	}

	//Write to RAM and ECHO RAM
	else if (0xE000 <= address && address < 0xFE00) {

		memory[address] = data;
		memory[address - 0x2000] = data;

	}

	//Prevent writing to unusable memory
	else if (0xFEA0 <= address && address < 0xFF00) {
	}

	//Joypad register
	else if (address == 0xFF00) {
		memory[address] = data & 0xF0;
	}

	//Divider register
	else if (address == 0xFF04) {
		memory[address] = 0;
		//reset dividercounter
	}

	//LY register
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

byte MMU::read(word address) {
	
	if (cartridge->mbc == MBC1) {

		if (address < 0x4000) {
			return (*cartridge)[address];
		}
		else if (address < 0x8000) {
			if (cartridge->mode){
				return (*cartridge)[address + (0x4000 * ((cartridge->bankNum & 0x1F) - 1))];
			}
			else {
				return (*cartridge)[address + (0x4000 * ((cartridge->bankNum & 0x7F) - 1))];
			}
		}
		else if (0xA000 <= address && address < 0xC000) {

			//RAM mode
			if (cartridge->mode) {
				return cartridge->ram[(address - 0xA000) + (((cartridge->bankNum >> 5) & 0x03) * 0x2000)];
			}
			//ROM mode
			else {
				return cartridge->ram[address - 0xA000];
			}

		}
	}

	if (cartridge->mbc == MBC2) {

		if (address < 0x4000) {
			return cartridge->rom[address];
		}
		else if (address < 0x8000) {
			return cartridge->rom[address + (0x4000 * ((cartridge->bankNum & 0x0F) - 1))];
		}
		else if (0xA000 <= address & address < 0xA200) {
			return (cartridge->ram[address - 0xA000] & 0x0F);
		}

	}

	if (address < 0x8000) {
		return (*cartridge)[address];
	}

	//Joypad register; bits 6-7 always return 1
	else if (address == 0xFF00) {
		return memory[address] | 0xC0;
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
