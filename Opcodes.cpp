#include "CPU.h"

byte CPU::execute(byte opcode) {
	switch (opcode) {
	case 0x00:
		return 4;
	case 0x01:
		reg.bc = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;
	case 0x02:
		mem.write(reg.bc, reg.a);
		return 8;
	case 0x03:
		reg.bc++;
		return 8;
	case 0x04:
		reg.b = INC_8bit(reg.b);
		return 4;
	case 0x05:
		reg.b = DEC_8bit(reg.b);
		return 4;
	case 0x06:
		reg.b = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x07:
		RLCA();
		return 4;
	case 0x08:
		mem.write(mem.read16(reg.pc), reg.sp & 0x00FF);
		mem.write(mem.read16(reg.pc) + 1, reg.sp >> 8);
		reg.pc += 2;
		return 20;
	case 0x09:
		ADD_16bit(reg.hl, reg.bc);
		return 8;
	case 0x0A:
		reg.a = mem.read(reg.bc);
		return 8;
	case 0x0B:
		reg.bc--;
		return 8;
	case 0x0C:
		reg.c = INC_8bit(reg.c);
		return 4;
	case 0x0D:
		reg.c = DEC_8bit(reg.c);
		return 4;
	case 0x0E:
		reg.c = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x0F:
		RRCA();
		return 4;
	case 0x11:
		reg.de = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;
	case 0x12:
		mem.write(reg.de, reg.a);
		return 8;
	case 0x13:
		reg.de++;
		return 8;
	case 0x14:
		reg.d = INC_8bit(reg.d);
		return 4;
	case 0x15:
		reg.d = DEC_8bit(reg.d);
		return 4;
	case 0x16:
		reg.d = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x17:
		RLA();
		return 4;
	case 0x18:
		JR((signed_byte)mem.read(reg.pc) + 1);
		return 12;
	case 0x19:
		ADD_16bit(reg.hl, reg.de);
		return 8;
	case 0x1A:
		reg.a = mem.read(reg.de);
		return 8;
	case 0x1B:
		reg.de--;
		return 8;
	case 0x1C:
		reg.e = INC_8bit(reg.e);
		return 4;
	case 0x1D:
		reg.e = DEC_8bit(reg.e);
		return 4;
	case 0x1E:
		reg.e = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x1F:
		RRA();
		return 4;
	case 0x20:
		if (reg.getFlag(ZFLAG)) {
			reg.pc++;
			return 8;
		}
		else {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}
	case 0x21:
		reg.hl = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;
	case 0x22:
		mem.write(reg.hl, reg.a);
		reg.hl++;
		return 8;
	case 0x23:
		reg.hl++;
		return 8;
	case 0x24:
		reg.h = INC_8bit(reg.h);
		return 4;
	case 0x25:
		reg.h = DEC_8bit(reg.h);
		return 4;
	case 0x26:
		reg.h = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x27:
		DAA();
		return 4;
	case 0x28:
		if (reg.getFlag(ZFLAG)) {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}
		else {
			reg.pc++;
			return 8;
		}
	case 0x29:
		ADD_16bit(reg.hl, reg.hl);
		return 8;
	case 0x2A:
		reg.a = mem.read(reg.hl);
		reg.hl++;
		return 8;
	case 0x2B:
		reg.hl--;
		return 8;
	case 0x2C:
		reg.l = INC_8bit(reg.l);
		return 4;
	case 0x2D:
		reg.l = DEC_8bit(reg.l);
		return 4;
	case 0x2E:
		reg.l = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x2F:
		reg.a = ~reg.a;
		reg.setFlag(HFLAG | NFLAG);
		return 4;
	case 0x30:
		if (reg.getFlag(CFLAG)) {
			reg.pc++;
			return 8;
		}
		else {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}
	case 0x31:
		reg.sp = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;
	case 0x32:
		mem.write(reg.hl, reg.a);
		reg.hl--;
		return 8;
	case 0x33:
		reg.sp++;
		return 8;
	case 0x34:
		mem.write(reg.hl, INC_8bit(mem.read(reg.hl)));
		return 12;
	case 0x35:
		mem.write(reg.hl, DEC_8bit(mem.read(reg.hl)));
		return 12;
	case 0x36:
		mem.write(reg.hl, mem.read(reg.pc));
		reg.pc++;
		return 12;
	case 0x37:
		reg.setFlag(CFLAG);
		reg.resetFlag(HFLAG | NFLAG);
		return 4;
	case 0x38:
		if (reg.getFlag(CFLAG)) {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}
		else {
			reg.pc++;
			return 8;
		}
	case 0x39:
		ADD_16bit(reg.hl, reg.sp);
		return 8;
	case 0x3A:
		reg.a = mem.read(reg.hl);
		reg.hl--;
		return 8;
	case 0x3B:
		reg.sp--;
		return 8;
	case 0x3C:
		reg.a = INC_8bit(reg.a);
		return 4;
	case 0x3D:
		reg.a = DEC_8bit(reg.a);
		return 4;
	case 0x3E:
		reg.a = mem.read(reg.pc);
		reg.pc++;
		return 8;
	case 0x3F:
		reg.toggleFlag(CFLAG);
		reg.resetFlag(HFLAG | NFLAG);
		return 4;
	case 0x40:
		return 4;
	case 0x41:
		reg.b = reg.c;
		return 4;
	case 0x42:
		reg.b = reg.d;
		return 4;
	case 0x43:
		reg.b = reg.e;
		return 4;
	case 0x44:
		reg.b = reg.h;
		return 4;
	case 0x45:
		reg.b = reg.l;
		return 4;
	case 0x46:
		reg.b = mem.read(reg.hl);
		return 8;
	case 0x47:
		reg.b = reg.a;
		return 4;
	case 0x48:
		reg.c = reg.b;
		return 4;
	case 0x49:
		return 4;
	case 0x4A:
		reg.c = reg.d;
		return 4;
	case 0x4B:
		reg.c = reg.e;
		return 4;
	case 0x4C:
		reg.c = reg.h;
		return 4;
	case 0x4D:
		reg.c = reg.l;
		return 4;
	case 0x4E:
		reg.c = mem.read(reg.hl);
		return 8;
	case 0x4F:
		reg.c = reg.a;
		return 4;
	case 0x50:
		reg.d = reg.b;
		return 4;
	case 0x51:
		reg.d = reg.c;
		return 4;
	case 0x52:
		return 4;
	case 0x53:
		reg.d = reg.e;
		return 4;
	case 0x54:
		reg.d = reg.h;
		return 4;
	case 0x55:
		reg.d = reg.l;
		return 4;
	case 0x56:
		reg.d = mem.read(reg.hl);
		return 8;
	case 0x57:
		reg.d = reg.a;
		return 4;
	case 0x58:
		reg.e = reg.b;
		return 4;
	case 0x59:
		reg.e = reg.c;
		return 4;
	case 0x5A:
		reg.e = reg.d;
		return 4;
	case 0x5B:
		return 4;
	case 0x5C:
		reg.e = reg.h;
		return 4;
	case 0x5D:
		reg.e = reg.l;
		return 4;
	case 0x5E:
		reg.e = mem.read(reg.hl);
		return 8;
	case 0x5F:
		reg.e = reg.a;
		return 4;
	case 0x60:
		reg.h = reg.b;
		return 4;
	case 0x61:
		reg.h = reg.c;
		return 4;
	case 0x62:
		reg.h = reg.d;
		return 4;
	case 0x63:
		reg.h = reg.e;
		return 4;
	case 0x64:
		return 4;
	case 0x65:
		reg.h = reg.l;
		return 4;
	case 0x66:
		reg.h = mem.read(reg.hl);
		return 8;
	case 0x67:
		reg.h = reg.a;
		return 4;
	case 0x68:
		reg.l = reg.b;
		return 4;
	case 0x69:
		reg.l = reg.c;
		return 4;
	case 0x6A:
		reg.l = reg.d;
		return 4;
	case 0x6B:
		reg.l = reg.e;
		return 4;
	case 0x6C:
		reg.l = reg.h;
		return 4;
	case 0x6D:
		return 4;
	case 0x6E:
		reg.l = mem.read(reg.hl);
		return 8;
	case 0x6F:
		reg.l = reg.a;
		return 4;
	case 0x70: // LD (HL),B
		mem.write(reg.hl, reg.b);
		return 8;
	case 0x71:
		mem.write(reg.hl, reg.c);
		return 8;
	case 0x72:
		mem.write(reg.hl, reg.d);
		return 8;
	case 0x73:
		mem.write(reg.hl, reg.e);
		return 8;
	case 0x74:
		mem.write(reg.hl, reg.h);
		return 8;
	case 0x75:
		mem.write(reg.hl, reg.l);
		return 8;
	case 0x76:
		halt = true;
		return 4;
	case 0x77:
		mem.write(reg.hl, reg.a);
		return 8;
	case 0x78:
		reg.a = reg.b;
		return 4;
	case 0x79:
		reg.a = reg.c;
		return 4;
	case 0x7A:
		reg.a = reg.d;
		return 4;
	case 0x7B:
		reg.a = reg.e;
		return 4;
	case 0x7C:
		reg.a = reg.h;
		return 4;
	case 0x7D:
		reg.a = reg.l;
		return 4;
	case 0x7E:
		reg.a = mem.read(reg.hl);
		return 8;
	case 0x7F:
		return 4;
	case 0x80:
		ADD_8bit(reg.b);
		return 4;
	case 0x81:
		ADD_8bit(reg.c);
		return 4;
	case 0x82:
		ADD_8bit(reg.d);
		return 4;
	case 0x83:
		ADD_8bit(reg.e);
		return 4;
	case 0x84:
		ADD_8bit(reg.h);
		return 4;
	case 0x85:
		ADD_8bit(reg.l);
		return 4;
	case 0x86:
		ADD_8bit(mem.read(reg.hl));
		return 8;
	case 0x87:
		ADD_8bit(reg.a);
		return 4;
	case 0x88:
		ADC(reg.b);
		return 4;
	case 0x89:
		ADC(reg.c);
		return 4;
	case 0x8A:
		ADC(reg.d);
		return 4;
	case 0x8B:
		ADC(reg.e);
		return 4;
	case 0x8C:
		ADC(reg.h);
		return 4;
	case 0x8D:
		ADC(reg.l);
		return 4;
	case 0x8E:
		ADC(mem.read(reg.hl));
		return 8;
	case 0x8F:
		ADC(reg.a);
		return 4;
	case 0x90:
		SUB(reg.b);
		return 4;
	case 0x91:
		SUB(reg.c);
		return 4;
	case 0x92:
		SUB(reg.d);
		return 4;
	case 0x93:
		SUB(reg.e);
		return 4;
	case 0x94:
		SUB(reg.h);
		return 4;
	case 0x95:
		SUB(reg.l);
		return 4;
	case 0x96:
		SUB(mem.read(reg.hl));
		return 8;
	case 0x97:
		SUB(reg.a);
		return 4;
	case 0x98:
		SBC(reg.b);
		return 4;
	case 0x99:
		SBC(reg.c);
		return 4;
	case 0x9A:
		SBC(reg.d);
		return 4;
	case 0x9B:
		SBC(reg.e);
		return 4;
	case 0x9C:
		SBC(reg.h);
		return 4;
	case 0x9D:
		SBC(reg.l);
		return 4;
	case 0x9E:
		SBC(mem.read(reg.hl));
		return 8;
	case 0x9F:
		SBC(reg.a);
		return 4;
	case 0xA0:
		AND(reg.b);
		return 4;
	case 0xA1:
		AND(reg.c);
		return 4;
	case 0xA2:
		AND(reg.d);
		return 4;
	case 0xA3:
		AND(reg.e);
		return 4;
	case 0xA4:
		AND(reg.h);
		return 4;
	case 0xA5:
		AND(reg.l);
		return 4;
	case 0xA6:
		AND(mem.read(reg.hl));
		return 8;
	case 0xA7:
		AND(reg.a);
		return 4;
	case 0xA8:
		XOR(reg.b);
		return 4;
	case 0xA9:
		XOR(reg.c);
		return 4;
	case 0xAA:
		XOR(reg.d);
		return 4;
	case 0xAB:
		XOR(reg.e);
		return 4;
	case 0xAC:
		XOR(reg.h);
		return 4;
	case 0xAD:
		XOR(reg.l);
		return 4;
	case 0xAE:
		XOR(mem.read(reg.hl));
		return 8;
	case 0xAF:
		XOR(reg.a);
		return 4;
	case 0xB0:
		OR(reg.b);
		return 4;
	case 0xB1:
		OR(reg.c);
		return 4;
	case 0xB2:
		OR(reg.d);
		return 4;
	case 0xB3:
		OR(reg.e);
		return 4;
	case 0xB4:
		OR(reg.h);
		return 4;
	case 0xB5:
		OR(reg.l);
		return 4;
	case 0xB6:
		OR(mem.read(reg.hl));
		return 8;
	case 0xB7:
		OR(reg.a);
		return 4;
	case 0xB8:
		CP(reg.b);
		return 4;
	case 0xB9:
		CP(reg.c);
		return 4;
	case 0xBA:
		CP(reg.d);
		return 4;
	case 0xBB:
		CP(reg.e);
		return 4;
	case 0xBC:
		CP(reg.h);
		return 4;
	case 0xBD:
		CP(reg.l);
		return 4;
	case 0xBE:
		CP(mem.read(reg.hl));
		return 8;
	case 0xBF:
		CP(reg.a);
		return 4;
	case 0xC0:
		if (reg.getFlag(ZFLAG)) {
			return 8;
		}
		else {
			RET();
			return 20;
		}
	case 0xC1:
		reg.c = mem.read(reg.sp);
		reg.b = mem.read(++reg.sp);
		reg.sp++;
		return 12;
	case 0xC2:
		if (reg.getFlag(ZFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}
	case 0xC3:
		JP(mem.read(reg.pc), mem.read(reg.pc + 1));
		return 16;
	case 0xC4:
		if (reg.getFlag(ZFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}
	case 0xC5:
		reg.sp--;
		mem.write(reg.sp, reg.b);
		reg.sp--;
		mem.write(reg.sp, reg.c);
		return 16;
	case 0xC6:
		ADD_8bit(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xC7:
		RST(0x00);
		return 16;
	case 0xC8:
		if (reg.getFlag(ZFLAG)) {
			RET();
			return 20;
		}
		else {
			return 8;
		}
	case 0xC9:
		RET();
		return 16;
	case 0xCA:
		if (reg.getFlag(ZFLAG)) {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}
		else {
			reg.pc += 2;
			return 12;
		}
	case 0xCB:
		return executeCB(mem.read(reg.pc));
	case 0xCC:
		if (reg.getFlag(ZFLAG)) {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}
		else {
			reg.pc += 2;
			return 12;
		}
	case 0xCD:
		CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
		return 24;
	case 0xCE:
		ADC(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xCF:
		RST(0x08);
		return 16;
	case 0xD0:
		if (reg.getFlag(CFLAG)) {
			return 8;
		}
		else {
			RET();
			return 20;
		}
	case 0xD1:
		reg.e = mem.read(reg.sp);
		reg.d = mem.read(++reg.sp);
		reg.sp++;
		return 12;
	case 0xD2:
		if (reg.getFlag(CFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}
	case 0xD4:
		if (reg.getFlag(CFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}
	case 0xD5:
		mem.write(--reg.sp, reg.d);
		mem.write(--reg.sp, reg.e);
		return 16;
	case 0xD6:
		SUB(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xD7:
		RST(0x10);
		return 16;
	case 0xD8:
		if (reg.getFlag(CFLAG)) {
			RET();
			return 20;
		}
		else {
			return 8;
		}
	case 0xD9:
		RETI();
		return 16;
	case 0xDA:
		if (reg.getFlag(CFLAG)) {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}
		else {
			reg.pc += 2;
			return 12;
		}
	case 0xDC:
		if (reg.getFlag(CFLAG)) {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}
		else {
			reg.pc += 2;
			return 12;
		}
	case 0xDE:
		SBC(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xDF:
		RST(0x18);
		return 16;
	case 0xE0:
		mem.write((0xFF00 + mem.read(reg.pc)), reg.a);
		reg.pc++;
		return 12;
	case 0xE1:
		reg.l = mem.read(reg.sp);
		reg.h = mem.read(++reg.sp);
		reg.sp++;
		return 12;
	case 0xE2:
		mem.write((0xFF00 + reg.c), reg.a);
		return 8;
	case 0xE5:
		mem.write(--reg.sp, reg.h);
		mem.write(--reg.sp, reg.l);
		return 16;
	case 0xE6:
		AND(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xE7:
		RST(0x20);
		return 16;
	case 0xE8:
		ADD_sp((signed_byte)mem.read(reg.pc));
		reg.pc++;
		return 16;
	case 0xE9:
		JP(reg.l, reg.h);
		return 4;
	case 0xEA: ///byte reversal
		mem.write((word)(mem.read(reg.pc + 1) << 8) | mem.read(reg.pc), reg.a);
		reg.pc += 2;
		return 16;
	case 0xEE:
		XOR(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xEF:
		RST(0x28);
		return 16;
	case 0xF0:
		reg.a = mem.read((0xFF00 + mem.read(reg.pc)));
		reg.pc++;
		return 12;
	case 0xF1:
		reg.f = mem.read(reg.sp);
		reg.f &= 0xF0;
		reg.a = mem.read(++reg.sp);
		reg.sp++;
		return 12;
	case 0xF2:
		reg.a = mem.read((0xFF00 + reg.c));
		return 8;
	case 0xF3:
		IME = 0;
		return 4;
	case 0xF5:
		mem.write(--reg.sp, reg.a);
		mem.write(--reg.sp, reg.f);
		return 16;
	case 0xF6:
		OR(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xF7:
		RST(0x30);
		return 16;
	case 0xF8:
		LDHL((signed_byte)mem.read(reg.pc));
		reg.pc++;
		return 12;
	case 0xF9:
		reg.sp = reg.hl;
		return 8;
	case 0xFA: //byte reversal
		reg.a = mem.read((mem.read(reg.pc + 1) << 8) | mem.read(reg.pc));
		reg.pc += 2;
		return 16;
	case 0xFB:
		IME = 1;
		return 4;
	case 0xFE:
		CP(mem.read(reg.pc));
		reg.pc++;
		return 8;
	case 0xFF:
		RST(0x38);
		return 16;


	}

}

byte CPU::executeCB(byte opcode)
{
	reg.pc++;
	switch (opcode) {
	case 0x00:
		reg.b = RLC(reg.b);
		return 8;
	case 0x01:
		reg.c = RLC(reg.c);
		return 8;
	case 0x02:
		reg.d = RLC(reg.d);
		return 8;
	case 0x03:
		reg.e = RLC(reg.e);
		return 8;
	case 0x04:
		reg.h = RLC(reg.h);
		return 8;
	case 0x05:
		reg.l = RLC(reg.l);
		return 8;
	case 0x06:
		mem.write(reg.hl, RLC(mem.read(reg.hl)));
		return 16;
	case 0x07:
		reg.a = RLC(reg.a);
		return 8;
	case 0x08:
		reg.b = RRC(reg.b);
		return 8;
	case 0x09:
		reg.c = RRC(reg.c);
		return 8;
	case 0x0A:
		reg.d = RRC(reg.d);
		return 8;
	case 0x0B:
		reg.e = RRC(reg.e);
		return 8;
	case 0x0C:
		reg.h = RRC(reg.h);
		return 8;
	case 0x0D:
		reg.l = RRC(reg.l);
		return 8;
	case 0x0E:
		mem.write(reg.hl, RRC(mem.read(reg.hl)));
		return 16;
	case 0x0F:
		reg.a = RRC(reg.a);
		return 8;
	case 0x10:
		reg.b = RL(reg.b);
		return 8;
	case 0x11:
		reg.c = RL(reg.c);
		return 8;
	case 0x12:
		reg.d = RL(reg.d);
		return 8;
	case 0x13:
		reg.e = RL(reg.e);
		return 8;
	case 0x14:
		reg.h = RL(reg.h);
		return 8;
	case 0x15:
		reg.l = RL(reg.l);
		return 8;
	case 0x16:
		mem.write(reg.hl, RL(mem.read(reg.hl)));
		return 16;
	case 0x17:
		reg.a = RL(reg.a);
		return 8;
	case 0x18:
		reg.b = RR(reg.b);
		return 8;
	case 0x19:
		reg.c = RR(reg.c);
		return 8;
	case 0x1A:
		reg.d = RR(reg.d);
		return 8;
	case 0x1B:
		reg.e = RR(reg.e);
		return 8;
	case 0x1C:
		reg.h = RR(reg.h);
		return 8;
	case 0x1D:
		reg.l = RR(reg.l);
		return 8;
	case 0x1E:
		mem.write(reg.hl, RR(mem.read(reg.hl)));
		return 16;
	case 0x1F:
		reg.a = RR(reg.a);
		return 8;
	case 0x20:
		reg.b = SLA(reg.b);
		return 8;
	case 0x21:
		reg.c = SLA(reg.c);
		return 8;
	case 0x22:
		reg.d = SLA(reg.d);
		return 8;
	case 0x23:
		reg.e = SLA(reg.e);
		return 8;
	case 0x24:
		reg.h = SLA(reg.h);
		return 8;
	case 0x25:
		reg.l = SLA(reg.l);
		return 8;
	case 0x26:
		mem.write(reg.hl, SLA(mem.read(reg.hl)));
		return 16;
	case 0x27:
		reg.a = SLA(reg.a);
		return 8;
	case 0x28:
		reg.b = SRA(reg.b);
		return 8;
	case 0x29:
		reg.c = SRA(reg.c);
		return 8;
	case 0x2A:
		reg.d = SRA(reg.d);
		return 8;
	case 0x2B:
		reg.e = SRA(reg.e);
		return 8;
	case 0x2C:
		reg.h = SRA(reg.h);
		return 8;
	case 0x2D:
		reg.l = SRA(reg.l);
		return 8;
	case 0x2E:
		mem.write(reg.hl, SRA(mem.read(reg.hl)));
		return 16;
	case 0x2F:
		reg.a = SRA(reg.a);
		return 8;
	case 0x30:
		reg.b = SWAP(reg.b);
		return 8;
	case 0x31:
		reg.c = SWAP(reg.c);
		return 8;
	case 0x32:
		reg.d = SWAP(reg.d);
		return 8;
	case 0x33:
		reg.e = SWAP(reg.e);
		return 8;
	case 0x34:
		reg.h = SWAP(reg.h);
		return 8;
	case 0x35:
		reg.l = SWAP(reg.l);
		return 8;
	case 0x36:
		mem.write(reg.hl, SWAP(mem.read(reg.hl)));
		return 16;
	case 0x37:
		reg.a = SWAP(reg.a);
		return 8;
	case 0x38:
		reg.b = SRL(reg.b);
		return 8;
	case 0x39:
		reg.c = SRL(reg.c);
		return 8;
	case 0x3A:
		reg.d = SRL(reg.d);
		return 8;
	case 0x3B:
		reg.e = SRL(reg.e);
		return 8;
	case 0x3C:
		reg.h = SRL(reg.h);
		return 8;
	case 0x3D:
		reg.l = SRL(reg.l);
		return 8;
	case 0x3E:
		mem.write(reg.hl, SRL(mem.read(reg.hl)));
		return 16;
	case 0x3F:
		reg.a = SRL(reg.a);
		return 8;
	case 0x40:
		BIT(reg.b, 0x01);
		return 8;
	case 0x41:
		BIT(reg.c, 0x01);
		return 8;
	case 0x42:
		BIT(reg.d, 0x01);
		return 8;
	case 0x43:
		BIT(reg.e, 0x01);
		return 8;
	case 0x44:
		BIT(reg.h, 0x01);
		return 8;
	case 0x45:
		BIT(reg.l, 0x01);
		return 8;
	case 0x46:
		BIT(mem.read(reg.hl), 0x01);
		return 12;
	case 0x47:
		BIT(reg.a, 0x01);
		return 8;
	case 0x48:
		BIT(reg.b, 0x02);
		return 8;
	case 0x49:
		BIT(reg.c, 0x02);
		return 8;
	case 0x4A:
		BIT(reg.d, 0x02);
		return 8;
	case 0x4B:
		BIT(reg.e, 0x02);
		return 8;
	case 0x4C:
		BIT(reg.h, 0x02);
		return 8;
	case 0x4D:
		BIT(reg.l, 0x02);
		return 8;
	case 0x4E:
		BIT(mem.read(reg.hl), 0x02);
		return 12;
	case 0x4F:
		BIT(reg.a, 0x02);
		return 8;
	case 0x50:
		BIT(reg.b, 0x04);
		return 8;
	case 0x51:
		BIT(reg.c, 0x04);
		return 8;
	case 0x52:
		BIT(reg.d, 0x04);
		return 8;
	case 0x53:
		BIT(reg.e, 0x04);
		return 8;
	case 0x54:
		BIT(reg.h, 0x04);
		return 8;
	case 0x55:
		BIT(reg.l, 0x04);
		return 8;
	case 0x56:
		BIT(mem.read(reg.hl), 0x04);
		return 12;
	case 0x57:
		BIT(reg.a, 0x04);
		return 8;
	case 0x58:
		BIT(reg.b, 0x08);
		return 8;
	case 0x59:
		BIT(reg.c, 0x08);
		return 8;
	case 0x5A:
		BIT(reg.d, 0x08);
		return 8;
	case 0x5B:
		BIT(reg.e, 0x08);
		return 8;
	case 0x5C:
		BIT(reg.h, 0x08);
		return 8;
	case 0x5D:
		BIT(reg.l, 0x08);
		return 8;
	case 0x5E:
		BIT(mem.read(reg.hl), 0x08);
		return 12;
	case 0x5F:
		BIT(reg.a, 0x08);
		return 8;
	case 0x60:
		BIT(reg.b, 0x10);
		return 8;
	case 0x61:
		BIT(reg.c, 0x10);
		return 8;
	case 0x62:
		BIT(reg.d, 0x10);
		return 8;
	case 0x63:
		BIT(reg.e, 0x10);
		return 8;
	case 0x64:
		BIT(reg.h, 0x10);
		return 8;
	case 0x65:
		BIT(reg.l, 0x10);
		return 8;
	case 0x66:
		BIT(mem.read(reg.hl), 0x10);
		return 12;
	case 0x67:
		BIT(reg.a, 0x10);
		return 8;
	case 0x68:
		BIT(reg.b, 0x20);
		return 8;
	case 0x69:
		BIT(reg.c, 0x20);
		return 8;
	case 0x6A:
		BIT(reg.d, 0x20);
		return 8;
	case 0x6B:
		BIT(reg.e, 0x20);
		return 8;
	case 0x6C:
		BIT(reg.h, 0x20);
		return 8;
	case 0x6D:
		BIT(reg.l, 0x20);
		return 8;
	case 0x6E:
		BIT(mem.read(reg.hl), 0x20);
		return 12;
	case 0x6F:
		BIT(reg.a, 0x20);
		return 8;
	case 0x70:
		BIT(reg.b, 0x40);
		return 8;
	case 0x71:
		BIT(reg.c, 0x40);
		return 8;
	case 0x72:
		BIT(reg.d, 0x40);
		return 8;
	case 0x73:
		BIT(reg.e, 0x40);
		return 8;
	case 0x74:
		BIT(reg.h, 0x40);
		return 8;
	case 0x75:
		BIT(reg.l, 0x40);
		return 8;
	case 0x76:
		BIT(mem.read(reg.hl), 0x40);
		return 12;
	case 0x77:
		BIT(reg.a, 0x40);
		return 8;
	case 0x78:
		BIT(reg.b, 0x80);
		return 8;
	case 0x79:
		BIT(reg.c, 0x80);
		return 8;
	case 0x7A:
		BIT(reg.d, 0x80);
		return 8;
	case 0x7B:
		BIT(reg.e, 0x80);
		return 8;
	case 0x7C:
		BIT(reg.h, 0x80);
		return 8;
	case 0x7D:
		BIT(reg.l, 0x80);
		return 8;
	case 0x7E:
		BIT(mem.read(reg.hl), 0x80);
		return 12;
	case 0x7F:
		BIT(reg.a, 0x80);
		return 8;
	case 0x80:
		reg.b &= ~(0x01);
		return 8;
	case 0x81:
		reg.c &= ~(0x01);
		return 8;
	case 0x82:
		reg.d &= ~(0x01);
		return 8;
	case 0x83:
		reg.e &= ~(0x01);
		return 8;
	case 0x84:
		reg.h &= ~(0x01);
		return 8;
	case 0x85:
		reg.l &= ~(0x01);
		return 8;
	case 0x86:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x01));
		return 16;
	case 0x87:
		reg.a &= ~(0x01);
		return 8;
	case 0x88:
		reg.b &= ~(0x02);
		return 8;
	case 0x89:
		reg.c &= ~(0x02);
		return 8;
	case 0x8A:
		reg.d &= ~(0x02);
		return 8;
	case 0x8B:
		reg.e &= ~(0x02);
		return 8;
	case 0x8C:
		reg.h &= ~(0x02);
		return 8;
	case 0x8D:
		reg.l &= ~(0x02);
		return 8;
	case 0x8E:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x02));
		return 16;
	case 0x8F:
		reg.a &= ~(0x02);
		return 8;
	case 0x90:
		reg.b &= ~(0x04);
		return 8;
	case 0x91:
		reg.c &= ~(0x04);
		return 8;
	case 0x92:
		reg.d &= ~(0x04);
		return 8;
	case 0x93:
		reg.e &= ~(0x04);
		return 8;
	case 0x94:
		reg.h &= ~(0x04);
		return 8;
	case 0x95:
		reg.l &= ~(0x04);
		return 8;
	case 0x96:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x04));
		return 16;
	case 0x97:
		reg.a &= ~(0x04);
		return 8;
	case 0x98:
		reg.b &= ~(0x08);
		return 8;
	case 0x99:
		reg.c &= ~(0x08);
		return 8;
	case 0x9A:
		reg.d &= ~(0x08);
		return 8;
	case 0x9B:
		reg.e &= ~(0x08);
		return 8;
	case 0x9C:
		reg.h &= ~(0x08);
		return 8;
	case 0x9D:
		reg.l &= ~(0x08);
		return 8;
	case 0x9E:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x08));
		return 16;
	case 0x9F:
		reg.a &= ~(0x08);
		return 8;
	case 0xA0:
		reg.b &= ~(0x10);
		return 8;
	case 0xA1:
		reg.c &= ~(0x10);
		return 8;
	case 0xA2:
		reg.d &= ~(0x10);
		return 8;
	case 0xA3:
		reg.e &= ~(0x10);
		return 8;
	case 0xA4:
		reg.h &= ~(0x10);
		return 8;
	case 0xA5:
		reg.l &= ~(0x10);
		return 8;
	case 0xA6:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x10));
		return 16;
	case 0xA7:
		reg.a &= ~(0x10);
		return 8;
	case 0xA8:
		reg.b &= ~(0x20);
		return 8;
	case 0xA9:
		reg.c &= ~(0x20);
		return 8;
	case 0xAA:
		reg.d &= ~(0x20);
		return 8;
	case 0xAB:
		reg.e &= ~(0x20);
		return 8;
	case 0xAC:
		reg.h &= ~(0x20);
		return 8;
	case 0xAD:
		reg.l &= ~(0x20);
		return 8;
	case 0xAE:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x20));
		return 16;
	case 0xAF:
		reg.a &= ~(0x20);
		return 8;
	case 0xB0:
		reg.b &= ~(0x40);
		return 8;
	case 0xB1:
		reg.c &= ~(0x40);
		return 8;
	case 0xB2:
		reg.d &= ~(0x40);
		return 8;
	case 0xB3:
		reg.e &= ~(0x40);
		return 8;
	case 0xB4:
		reg.h &= ~(0x40);
		return 8;
	case 0xB5:
		reg.l &= ~(0x40);
		return 8;
	case 0xB6:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x40));
		return 16;
	case 0xB7:
		reg.a &= ~(0x40);
		return 8;
	case 0xB8:
		reg.b &= ~(0x80);
		return 8;
	case 0xB9:
		reg.c &= ~(0x80);
		return 8;
	case 0xBA:
		reg.d &= ~(0x80);
		return 8;
	case 0xBB:
		reg.e &= ~(0x80);
		return 8;
	case 0xBC:
		reg.h &= ~(0x80);
		return 8;
	case 0xBD:
		reg.l &= ~(0x80);
		return 8;
	case 0xBE:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x80));
		return 16;
	case 0xBF:
		reg.a &= ~(0x80);
		return 8;
	case 0xC0:
		reg.b |= 0x01;
		return 8;
	case 0xC1:
		reg.c |= 0x01;
		return 8;
	case 0xC2:
		reg.d |= 0x01;
		return 8;
	case 0xC3:
		reg.e |= 0x01;
		return 8;
	case 0xC4:
		reg.h |= 0x01;
		return 8;
	case 0xC5:
		reg.l |= 0x01;
		return 8;
	case 0xC6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x01);
		return 16;
	case 0xC7:
		reg.a |= 0x01;
		return 8;
	case 0xC8:
		reg.b |= 0x02;
		return 8;
	case 0xC9:
		reg.c |= 0x02;
		return 8;
	case 0xCA:
		reg.d |= 0x02;
		return 8;
	case 0xCB:
		reg.e |= 0x02;
		return 8;
	case 0xCC:
		reg.h |= 0x02;
		return 8;
	case 0xCD:
		reg.l |= 0x02;
		return 8;
	case 0xCE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x02);
		return 16;
	case 0xCF:
		reg.a |= 0x02;
		return 8;
	case 0xD0:
		reg.b |= 0x04;
		return 8;
	case 0xD1:
		reg.c |= 0x04;
		return 8;
	case 0xD2:
		reg.d |= 0x04;
		return 8;
	case 0xD3:
		reg.e |= 0x04;
		return 8;
	case 0xD4:
		reg.h |= 0x04;
		return 8;
	case 0xD5:
		reg.l |= 0x04;
		return 8;
	case 0xD6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x04);
		return 16;
	case 0xD7:
		reg.a |= 0x04;
		return 8;
	case 0xD8:
		reg.b |= 0x08;
		return 8;
	case 0xD9:
		reg.c |= 0x08;
		return 8;
	case 0xDA:
		reg.d |= 0x08;
		return 8;
	case 0xDB:
		reg.e |= 0x08;
		return 8;
	case 0xDC:
		reg.h |= 0x08;
		return 8;
	case 0xDD:
		reg.l |= 0x08;
		return 8;
	case 0xDE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x08);
		return 16;
	case 0xDF:
		reg.a |= 0x08;
		return 8;
	case 0xE0:
		reg.b |= 0x10;
		return 8;
	case 0xE1:
		reg.c |= 0x10;
		return 8;
	case 0xE2:
		reg.d |= 0x10;
		return 8;
	case 0xE3:
		reg.e |= 0x10;
		return 8;
	case 0xE4:
		reg.h |= 0x10;
		return 8;
	case 0xE5:
		reg.l |= 0x10;
		return 8;
	case 0xE6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x10);
		return 16;
	case 0xE7:
		reg.a |= 0x10;
		return 8;
	case 0xE8:
		reg.b |= 0x20;
		return 8;
	case 0xE9:
		reg.c |= 0x20;
		return 8;
	case 0xEA:
		reg.d |= 0x20;
		return 8;
	case 0xEB:
		reg.e |= 0x20;
		return 8;
	case 0xEC:
		reg.h |= 0x20;
		return 8;
	case 0xED:
		reg.l |= 0x20;
		return 8;
	case 0xEE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x20);
		return 16;
	case 0xEF:
		reg.a |= 0x20;
		return 8;
	case 0xF0:
		reg.b |= 0x40;
		return 8;
	case 0xF1:
		reg.c |= 0x40;
		return 8;
	case 0xF2:
		reg.d |= 0x40;
		return 8;
	case 0xF3:
		reg.e |= 0x40;
		return 8;
	case 0xF4:
		reg.h |= 0x40;
		return 8;
	case 0xF5:
		reg.l |= 0x40;
		return 8;
	case 0xF6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x40);
		return 16;
	case 0xF7:
		reg.a |= 0x40;
		return 8;
	case 0xF8:
		reg.b |= 0x80;
		return 8;
	case 0xF9:
		reg.c |= 0x80;
		return 8;
	case 0xFA:
		reg.d |= 0x80;
		return 8;
	case 0xFB:
		reg.e |= 0x80;
		return 8;
	case 0xFC:
		reg.h |= 0x80;
		return 8;
	case 0xFD:
		reg.l |= 0x80;
		return 8;
	case 0xFE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x80);
		return 16;
	case 0xFF:
		reg.a |= 0x80;
		return 8;

	}

}