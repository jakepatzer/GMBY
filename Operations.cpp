#include "CPU.h"
#include<stdio.h>


void CPU::ADD_8bit(byte value)
{

	reg.resetFlag(NFLAG);

	if (((reg.a & 0xF) + (value & 0xF)) & 0x10) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (reg.a + value > 255) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

	reg.a = reg.a + value;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::ADD_16bit(word & dest, word value)
{

	reg.resetFlag(NFLAG);

	if (((dest & 0xFFF) + (value & 0xFFF)) & 0x1000) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (dest + value > 0xFFFF) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

	dest += value;

}

void CPU::ADD_sp(signed_byte value)
{

	reg.resetFlag(ZFLAG);
	reg.resetFlag(NFLAG);

	if (((reg.sp & 0xFFF) + value) & 0x1000) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (reg.sp + value > 0xFFFF) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

	reg.sp += value;

}

void CPU::ADC(byte value)
{

	byte c = 0;
	if (reg.getFlag(CFLAG)) {
		c = 1;
	}

	reg.resetFlag(NFLAG);

	if (((reg.a & 0xF) + (value & 0xF) + c) & 0x10) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (reg.a + value + c > 255) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

	reg.a = reg.a + value + c;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}
}

void CPU::CP(byte value)
{

	reg.setFlag(NFLAG);

	if (reg.a == value) {
		reg.setFlag(ZFLAG);
	}
	else {
		reg.resetFlag(ZFLAG);
	}

	if (((reg.a & 0xF) - (value & 0xF)) < 0) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (reg.a - value < 0) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

}

void CPU::SUB(byte value)
{

	reg.setFlag(NFLAG);

	if (((reg.a & 0xF) - (value & 0xF)) < 0) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (reg.a - value < 0) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

	reg.a -= value;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::SBC(byte value)
{

	byte c = 0;
	if (reg.getFlag(CFLAG)) {
		c = 1;
	}

	reg.setFlag(NFLAG);

	if (((reg.a & 0xF) - (value & 0xF) - c) < 0) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	if (reg.a - value - c < 0) {
		reg.setFlag(CFLAG);
	}
	else {
		reg.resetFlag(CFLAG);
	}

	reg.a = reg.a - value - c;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::AND(byte value)
{

	reg.resetFlag(CFLAG);
	reg.setFlag(HFLAG);
	reg.resetFlag(NFLAG);

	reg.a &= value;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::XOR(byte value)
{

	reg.resetFlag(CFLAG);
	reg.resetFlag(HFLAG);
	reg.resetFlag(NFLAG);

	reg.a ^= value;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::OR(byte value)
{

	reg.resetFlag(CFLAG);
	reg.resetFlag(HFLAG);
	reg.resetFlag(NFLAG);

	reg.a |= value;

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

byte CPU::INC_8bit(byte value)
{
	reg.resetFlag(NFLAG);

	if (((value & 0xF) + 1) & 0x10) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	byte result = value + 1;

	if (result) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

	return result;

}

byte CPU::DEC_8bit(byte value)
{

	reg.setFlag(NFLAG);

	if ((value & 0xF) == 0) {
		reg.setFlag(HFLAG);
	}
	else {
		reg.resetFlag(HFLAG);
	}

	byte result = value - 1;

	if (result) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

	return result;

}

void CPU::DAA()
{

	if (reg.getFlag(NFLAG)) { //SUB
		if (reg.getFlag(CFLAG)) { //c=1

			if (reg.getFlag(HFLAG)) { //h=1
				reg.a += 0x9A;
			}
			else { //h=0
				reg.a += 0xA0;
			}
			reg.setFlag(CFLAG);
		}
		else { //c=0
			if (reg.getFlag(HFLAG)) {
				reg.a += 0xFA;
			}
			reg.resetFlag(CFLAG);
		}
	}
	else { //ADD

		if (reg.getFlag(CFLAG)) {

			if (reg.getFlag(HFLAG)) {
				reg.a += 0x66;
			}
			else {

				if ((reg.a & 0xF) <= 0x9) {
					reg.a += 0x60;
				}
				else {
					reg.a += 0x66;
				}

			}
			reg.setFlag(CFLAG);
		}
		else {

			if (reg.getFlag(HFLAG)) {

				if ((reg.a & 0xF0) <= 0x9) {
					reg.a += 0x06;
					reg.resetFlag(CFLAG);
				}
				else {
					reg.a += 0x66;
					reg.setFlag(CFLAG);
				}

			}
			else {

				if ((reg.a & 0x0F) <= 0x9) {

					if ((reg.a & 0xF0) <= 0x9) {
						reg.resetFlag(CFLAG);
					}
					else {
						reg.a += 0x60;
						reg.setFlag(CFLAG);
					}

				}
				else {

					if ((reg.a & 0xF0) <= 0x8) {
						reg.a += 0x06;
						reg.resetFlag(CFLAG);
					}
					else {
						reg.a += 0x66;
						reg.setFlag(CFLAG);
					}

				}

			}

		}

	}

	reg.resetFlag(HFLAG);

	if (reg.a) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::RLCA()
{
	reg.a = (reg.a << 1) | ((reg.a >> 7) & 1);
	if (reg.a & 1) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }
	reg.resetFlag(NFLAG | HFLAG | ZFLAG);
}

byte CPU::RLC(byte value)
{
	value = (value << 1) | ((value >> 7) & 1);

	if (value & 1) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }

	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }

	reg.resetFlag(NFLAG | HFLAG);
	return value;
}

void CPU::RLA()
{
	byte c = reg.a & 0x80;
	byte cflag = 0;

	if (reg.getFlag(CFLAG)) { cflag = 1; }

	reg.a = (reg.a << 1) | cflag;

	if (c) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }

	reg.resetFlag(NFLAG | HFLAG | ZFLAG);
}

byte CPU::RL(byte value)
{
	byte c = value & 0x80;
	byte cflag = 0;

	if (reg.getFlag(CFLAG)) { cflag = 1; }

	value = (value << 1) | cflag;

	if (c) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }

	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }

	reg.resetFlag(NFLAG | HFLAG);
	return value;
}

void CPU::RRCA()
{
	reg.a = (reg.a >> 1) | ((reg.a << 7) & 0x80);

	if (reg.a & 0x80) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }

	reg.resetFlag(NFLAG | HFLAG | ZFLAG);
}

byte CPU::RRC(byte value)
{
	value = (value >> 1) | ((value << 7) & 0x80);

	if (value & 0x80) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }

	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }

	reg.resetFlag(NFLAG | HFLAG);
	return value;
}

void CPU::RRA()
{
	byte c = reg.a & 1;
	byte cflag = 0;
	if (reg.getFlag(CFLAG)) { cflag = 1; }
	reg.a = (reg.a >> 1) | (cflag << 7);
	if (c) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }
	reg.resetFlag(NFLAG | HFLAG | ZFLAG);
}

byte CPU::RR(byte value)
{
	byte c = value & 1;
	bool cflag = reg.getFlag(CFLAG);

	value >>= 1;
	if (reg.getFlag(CFLAG)) { value = bitSet(value, 7); }

	if (c) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }

	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }

	reg.resetFlag(NFLAG | HFLAG);
	return value;
}

byte CPU::SLA(byte value)
{
	reg.resetFlag(HFLAG | NFLAG);
	if (value & 0x80) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }
	value <<= 1;
	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }
	return value;
}

byte CPU::SRA(byte value)
{
	reg.resetFlag(HFLAG | NFLAG);
	if (value & 0x1) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }
	byte temp = value & 0x80;
	value >>= 1;
	value |= temp;
	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }
	return value;
}

byte CPU::SRL(byte value)
{
	reg.resetFlag(HFLAG | NFLAG);
	if (value & 0x1) { reg.setFlag(CFLAG); }
	else { reg.resetFlag(CFLAG); }
	value >>= 1;
	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }
	return value;
}

byte CPU::SWAP(byte value)
{
	reg.resetFlag(CFLAG | HFLAG | NFLAG);

	if (value) { reg.resetFlag(ZFLAG); }
	else { reg.setFlag(ZFLAG); }

	byte upper = value >> 4;
	value <<= 4;
	return value | upper;
}

void CPU::BIT(byte value, byte mask)
{

	reg.setFlag(HFLAG);
	reg.resetFlag(NFLAG);

	if (value & mask) {
		reg.resetFlag(ZFLAG);
	}
	else {
		reg.setFlag(ZFLAG);
	}

}

void CPU::JP(byte addressL, byte addressH)
{

	reg.pc = (addressH << 8) | addressL;

}

void CPU::JR(signed_byte offset)
{
	reg.pc += offset;
}

void CPU::RST(byte low)
{

	mem.write(reg.sp - 1, reg.pc >> 8);
	mem.write(reg.sp - 2, reg.pc & 0xFF);
	reg.sp -= 2;
	reg.pc = low;

}

void CPU::LDHL(signed_byte e)
{

	reg.resetFlag(NFLAG | ZFLAG);
	byte e_u = abs(e);
	reg.hl = reg.sp + e;
	if (e >= 0) {
		if ((reg.sp & 0xFF) + e > 0xFF) {
			reg.setFlag(CFLAG);
		}
		else {
			reg.resetFlag(CFLAG);
		}
		if ((reg.sp & 0xF) + (e & 0xF) > 0xF) {
			reg.setFlag(HFLAG);
		}
		else {
			reg.resetFlag(HFLAG);
		}
	}
	else {
		if ((reg.sp & 0xFF) > e_u) {
			reg.resetFlag(CFLAG);
		}
		else {
			reg.setFlag(CFLAG);
		}
		if ((reg.sp & 0xF) > (e_u & 0xF)) {
			reg.resetFlag(HFLAG);
		}
		else {
			reg.setFlag(HFLAG);
		}
	}

}

void CPU::requestInterrupt(byte bit)
{
	mem.write(IF, mem.read(IF) | bit);
}

void CPU::CALL(byte valueL, byte valueH)
{

	mem.write(reg.sp - 1, (reg.pc + 2) >> 8);
	mem.write(reg.sp - 2, (reg.pc + 2) & 0x00FF);
	reg.pc = (valueH << 8) | valueL;
	reg.sp -= 2;

}

void CPU::RET()
{

	reg.pc = (mem.read(reg.sp + 1) << 8) | mem.read(reg.sp);
	reg.sp += 2;

}

void CPU::RETI()
{

	reg.pc = (mem.read(reg.sp + 1) << 8) | mem.read(reg.sp);
	reg.sp += 2;
	IME = true;

}

byte CPU::bitSelect(byte value, byte bit) {

	return (value >> bit) & 0x01;

}

byte CPU::bitSet(byte value, byte bit) {

	byte mask = 1 << bit;
	return value | mask;

}

byte CPU::bitReset(byte value, byte bit) {

	byte mask = ~(1 << bit);
	return value & mask;

}