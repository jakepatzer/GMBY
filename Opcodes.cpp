#include "CPU.h"

byte CPU::execute(byte opcode) {
	switch (opcode) {

	//NOP
	case 0x00:
		return 4;

	//LD BC, d16
	case 0x01:
		reg.bc = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;

	//LD (BC), A
	case 0x02:
		mem.write(reg.bc, reg.a);
		return 8;

	//INC BC
	case 0x03:
		reg.bc++;
		return 8;
	
	//INC B
	case 0x04:
		reg.b = INC_8bit(reg.b);
		return 4;
	
	//DEC B
	case 0x05:
		reg.b = DEC_8bit(reg.b);
		return 4;
	
	//LD B, d8
	case 0x06:
		reg.b = mem.read(reg.pc);
		reg.pc++;
		return 8;
	
	//RLCA
	case 0x07:
		RLCA();
		return 4;
	
	//LD (a16), SP
	case 0x08:
		mem.write(mem.read16(reg.pc), reg.sp & 0x00FF);
		mem.write(mem.read16(reg.pc) + 1, reg.sp >> 8);
		reg.pc += 2;
		return 20;

	//ADD HL, BC
	case 0x09:
		ADD_16bit(reg.hl, reg.bc);
		return 8;

	//LD A, (BC)
	case 0x0A:
		reg.a = mem.read(reg.bc);
		return 8;

	//DEC BC
	case 0x0B:
		reg.bc--;
		return 8;
	
	//INC C
	case 0x0C:
		reg.c = INC_8bit(reg.c);
		return 4;
	
	//DEC C
	case 0x0D:
		reg.c = DEC_8bit(reg.c);
		return 4;
	
	//LD C, d8
	case 0x0E:
		reg.c = mem.read(reg.pc);
		reg.pc++;
		return 8;

	//RRCA
	case 0x0F:
		RRCA();
		return 4;
	
	//LD DE, d16
	case 0x11:
		reg.de = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;

	//LD (DE), A
	case 0x12:
		mem.write(reg.de, reg.a);
		return 8;
	
	//INC DE
	case 0x13:
		reg.de++;
		return 8;
	
	//INC D
	case 0x14:
		reg.d = INC_8bit(reg.d);
		return 4;

	//DEC D
	case 0x15:
		reg.d = DEC_8bit(reg.d);
		return 4;

	//LD D, d8
	case 0x16:
		reg.d = mem.read(reg.pc);
		reg.pc++;
		return 8;

	//RLA
	case 0x17:
		RLA();
		return 4;

	//JR r8
	case 0x18:
		JR((signed_byte)mem.read(reg.pc) + 1);
		return 12;

	//ADD HL, DE
	case 0x19:
		ADD_16bit(reg.hl, reg.de);
		return 8;

	//LD A, (DE)
	case 0x1A:
		reg.a = mem.read(reg.de);
		return 8;

	//DEC DE
	case 0x1B:
		reg.de--;
		return 8;
	
	//INC E
	case 0x1C:
		reg.e = INC_8bit(reg.e);
		return 4;

	//DEC E
	case 0x1D:
		reg.e = DEC_8bit(reg.e);
		return 4;
	
	//LD E, d8
	case 0x1E:
		reg.e = mem.read(reg.pc);
		reg.pc++;
		return 8;

	//RRA
	case 0x1F:
		RRA();
		return 4;
	
	//JR NZ, r8
	case 0x20:
		if (reg.getFlag(ZFLAG)) {
			reg.pc++;
			return 8;
		}
		else {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}

	//LD HL, d16
	case 0x21:
		reg.hl = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;

	//LD (HL+), A
	case 0x22:
		mem.write(reg.hl, reg.a);
		reg.hl++;
		return 8;

	//INC HL
	case 0x23:
		reg.hl++;
		return 8;

	//INC H
	case 0x24:
		reg.h = INC_8bit(reg.h);
		return 4;

	//DEC H
	case 0x25:
		reg.h = DEC_8bit(reg.h);
		return 4;

	//LD H, d8
	case 0x26:
		reg.h = mem.read(reg.pc);
		reg.pc++;
		return 8;

	//DAA
	case 0x27:
		DAA();
		return 4;

	//JR Z, r8
	case 0x28:
		if (reg.getFlag(ZFLAG)) {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}
		else {
			reg.pc++;
			return 8;
		}

	//ADD HL, HL
	case 0x29:
		ADD_16bit(reg.hl, reg.hl);
		return 8;

	//LD A, (HL+)
	case 0x2A:
		reg.a = mem.read(reg.hl);
		reg.hl++;
		return 8;

	//DEC HL
	case 0x2B:
		reg.hl--;
		return 8;

	//INC L
	case 0x2C:
		reg.l = INC_8bit(reg.l);
		return 4;

	//DEC L
	case 0x2D:
		reg.l = DEC_8bit(reg.l);
		return 4;

	//LD L, d8
	case 0x2E:
		reg.l = mem.read(reg.pc);
		reg.pc++;
		return 8;

	//CPL
	case 0x2F:
		reg.a = ~reg.a;
		reg.setFlag(HFLAG | NFLAG);
		return 4;

	//JR NC, r8
	case 0x30:
		if (reg.getFlag(CFLAG)) {
			reg.pc++;
			return 8;
		}
		else {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}

	//LD SP, d16
	case 0x31:
		reg.sp = mem.read16(reg.pc);
		reg.pc += 2;
		return 12;

	//LD (HL-), A
	case 0x32:
		mem.write(reg.hl, reg.a);
		reg.hl--;
		return 8;

	//INC SP
	case 0x33:
		reg.sp++;
		return 8;

	//INC (HL)
	case 0x34:
		mem.write(reg.hl, INC_8bit(mem.read(reg.hl)));
		return 12;

	//DEC (HL)
	case 0x35:
		mem.write(reg.hl, DEC_8bit(mem.read(reg.hl)));
		return 12;
	
	//LD (HL), d8
	case 0x36:
		mem.write(reg.hl, mem.read(reg.pc));
		reg.pc++;
		return 12;
	
	//SCF
	case 0x37:
		reg.setFlag(CFLAG);
		reg.resetFlag(HFLAG | NFLAG);
		return 4;
	
	//JR C, r8
	case 0x38:
		if (reg.getFlag(CFLAG)) {
			JR((signed_byte)mem.read(reg.pc) + 1);
			return 12;
		}
		else {
			reg.pc++;
			return 8;
		}
	
	//ADD HL, SP
	case 0x39:
		ADD_16bit(reg.hl, reg.sp);
		return 8;
	
	//LD A, (HL-)
	case 0x3A:
		reg.a = mem.read(reg.hl);
		reg.hl--;
		return 8;
	
	//DEC SP
	case 0x3B:
		reg.sp--;
		return 8;
	
	//INC A
	case 0x3C:
		reg.a = INC_8bit(reg.a);
		return 4;
	
	//DEC A
	case 0x3D:
		reg.a = DEC_8bit(reg.a);
		return 4;
	
	//LD A, d8
	case 0x3E:
		reg.a = mem.read(reg.pc);
		reg.pc++;
		return 8;
	
	//CCF
	case 0x3F:
		reg.toggleFlag(CFLAG);
		reg.resetFlag(HFLAG | NFLAG);
		return 4;
	
	//LD B, B
	case 0x40:
		return 4;
	
	//LD B, C
	case 0x41:
		reg.b = reg.c;
		return 4;
	
	//LD B, D
	case 0x42:
		reg.b = reg.d;
		return 4;
	
	//LD B, E
	case 0x43:
		reg.b = reg.e;
		return 4;
	
	//LD B, H
	case 0x44:
		reg.b = reg.h;
		return 4;
	
	//LD B, L
	case 0x45:
		reg.b = reg.l;
		return 4;

	//LD B, (HL)
	case 0x46:
		reg.b = mem.read(reg.hl);
		return 8;

	//LD B, A
	case 0x47:
		reg.b = reg.a;
		return 4;

	//LD C, B
	case 0x48:
		reg.c = reg.b;
		return 4;

	//LD C, C
	case 0x49:
		return 4;

	//LD C, D
	case 0x4A:
		reg.c = reg.d;
		return 4;

	//LD C, E
	case 0x4B:
		reg.c = reg.e;
		return 4;

	//LD C, H
	case 0x4C:
		reg.c = reg.h;
		return 4;

	//LD C, L
	case 0x4D:
		reg.c = reg.l;
		return 4;

	//LD C, (HL)
	case 0x4E:
		reg.c = mem.read(reg.hl);
		return 8;
	
	//LD C, A
	case 0x4F:
		reg.c = reg.a;
		return 4;

	//LD D, B
	case 0x50:
		reg.d = reg.b;
		return 4;

	//LD D, C
	case 0x51:
		reg.d = reg.c;
		return 4;

	//LD D, D
	case 0x52:
		return 4;

	//LD D, E
	case 0x53:
		reg.d = reg.e;
		return 4;

	//LD D, H
	case 0x54:
		reg.d = reg.h;
		return 4;

	//LD D, L
	case 0x55:
		reg.d = reg.l;
		return 4;

	//LD D, (HL)
	case 0x56:
		reg.d = mem.read(reg.hl);
		return 8;

	//LD D, A
	case 0x57:
		reg.d = reg.a;
		return 4;

	//LD E, B
	case 0x58:
		reg.e = reg.b;
		return 4;

	//LD E, C
	case 0x59:
		reg.e = reg.c;
		return 4;

	//LD E, D
	case 0x5A:
		reg.e = reg.d;
		return 4;

	//LD E, E
	case 0x5B:
		return 4;

	//LD E, H
	case 0x5C:
		reg.e = reg.h;
		return 4;

	//LD E, L
	case 0x5D:
		reg.e = reg.l;
		return 4;

	//LD E, (HL)
	case 0x5E:
		reg.e = mem.read(reg.hl);
		return 8;

	//LD E, A
	case 0x5F:
		reg.e = reg.a;
		return 4;

	//LD H, B
	case 0x60:
		reg.h = reg.b;
		return 4;

	//LD H, C
	case 0x61:
		reg.h = reg.c;
		return 4;

	//LD H, D
	case 0x62:
		reg.h = reg.d;
		return 4;

	//LD H, E
	case 0x63:
		reg.h = reg.e;
		return 4;

	//LD H, H
	case 0x64:
		return 4;

	//LD H, L
	case 0x65:
		reg.h = reg.l;
		return 4;

	//LD H, (HL)
	case 0x66:
		reg.h = mem.read(reg.hl);
		return 8;

	//LD H, A
	case 0x67:
		reg.h = reg.a;
		return 4;

	//LD L, B
	case 0x68:
		reg.l = reg.b;
		return 4;

	//LD L, C
	case 0x69:
		reg.l = reg.c;
		return 4;

	//LD L, D
	case 0x6A:
		reg.l = reg.d;
		return 4;

	//LD L, E
	case 0x6B:
		reg.l = reg.e;
		return 4;

	//LD L, H
	case 0x6C:
		reg.l = reg.h;
		return 4;

	//LD L, L
	case 0x6D:
		return 4;

	//LD L, (HL)
	case 0x6E:
		reg.l = mem.read(reg.hl);
		return 8;

	//LD L, A
	case 0x6F:
		reg.l = reg.a;
		return 4;

	//LD (HL), B
	case 0x70: 
		mem.write(reg.hl, reg.b);
		return 8;

	//LD (HL), C
	case 0x71:
		mem.write(reg.hl, reg.c);
		return 8;

	//LD (HL), D
	case 0x72:
		mem.write(reg.hl, reg.d);
		return 8;

	//LD (HL), E
	case 0x73:
		mem.write(reg.hl, reg.e);
		return 8;

	//LD (HL), H
	case 0x74:
		mem.write(reg.hl, reg.h);
		return 8;

	//LD (HL), L
	case 0x75:
		mem.write(reg.hl, reg.l);
		return 8;

	//HALT
	case 0x76:
		halt = true;
		return 4;

	//LD (HL), A
	case 0x77:
		mem.write(reg.hl, reg.a);
		return 8;

	//LD A, B
	case 0x78:
		reg.a = reg.b;
		return 4;

	//LD A, C
	case 0x79:
		reg.a = reg.c;
		return 4;

	//LD A, D
	case 0x7A:
		reg.a = reg.d;
		return 4;

	//LD A, E
	case 0x7B:
		reg.a = reg.e;
		return 4;

	//LD A, H
	case 0x7C:
		reg.a = reg.h;
		return 4;

	//LD A, L
	case 0x7D:
		reg.a = reg.l;
		return 4;

	//LD A, (HL)
	case 0x7E:
		reg.a = mem.read(reg.hl);
		return 8;

	//LD A, A
	case 0x7F:
		return 4;

	//ADD A, B
	case 0x80:
		ADD_8bit(reg.b);
		return 4;

	//ADD A, C
	case 0x81:
		ADD_8bit(reg.c);
		return 4;

	//ADD A, D
	case 0x82:
		ADD_8bit(reg.d);
		return 4;

	//ADD A, E
	case 0x83:
		ADD_8bit(reg.e);
		return 4;

	//ADD A, H
	case 0x84:
		ADD_8bit(reg.h);
		return 4;

	//ADD A, L
	case 0x85:
		ADD_8bit(reg.l);
		return 4;

	//ADD A, (HL)
	case 0x86:
		ADD_8bit(mem.read(reg.hl));
		return 8;

	//ADD A, A
	case 0x87:
		ADD_8bit(reg.a);
		return 4;

	//ADC A, B
	case 0x88:
		ADC(reg.b);
		return 4;

	//ADC A, C
	case 0x89:
		ADC(reg.c);
		return 4;

	//ADC A, D
	case 0x8A:
		ADC(reg.d);
		return 4;

	//ADC A, E
	case 0x8B:
		ADC(reg.e);
		return 4;

	//ADC A, H
	case 0x8C:
		ADC(reg.h);
		return 4;

	//ADC A, L
	case 0x8D:
		ADC(reg.l);
		return 4;

	//ADC A, (HL)
	case 0x8E:
		ADC(mem.read(reg.hl));
		return 8;

	//ADC A, A
	case 0x8F:
		ADC(reg.a);
		return 4;

	//SUB B
	case 0x90:
		SUB(reg.b);
		return 4;

	//SUB C
	case 0x91:
		SUB(reg.c);
		return 4;

	//SUB D
	case 0x92:
		SUB(reg.d);
		return 4;

	//SUB E
	case 0x93:
		SUB(reg.e);
		return 4;

	//SUB H
	case 0x94:
		SUB(reg.h);
		return 4;

	//SUB L
	case 0x95:
		SUB(reg.l);
		return 4;

	//SUB (HL)
	case 0x96:
		SUB(mem.read(reg.hl));
		return 8;

	//SUB A
	case 0x97:
		SUB(reg.a);
		return 4;

	//SBC A, B
	case 0x98:
		SBC(reg.b);
		return 4;

	//SBC A, C
	case 0x99:
		SBC(reg.c);
		return 4;

	//SBC A, D
	case 0x9A:
		SBC(reg.d);
		return 4;

	//SBC A, E
	case 0x9B:
		SBC(reg.e);
		return 4;

	//SBC A, H
	case 0x9C:
		SBC(reg.h);
		return 4;

	//SBC A, L
	case 0x9D:
		SBC(reg.l);
		return 4;

	//SBC A, (HL)
	case 0x9E:
		SBC(mem.read(reg.hl));
		return 8;

	//SBC A, A
	case 0x9F:
		SBC(reg.a);
		return 4;

	//AND B
	case 0xA0:
		AND(reg.b);
		return 4;
	
	//AND C
	case 0xA1:
		AND(reg.c);
		return 4;

	//AND D
	case 0xA2:
		AND(reg.d);
		return 4;

	//AND E
	case 0xA3:
		AND(reg.e);
		return 4;

	//AND H
	case 0xA4:
		AND(reg.h);
		return 4;

	//AND L
	case 0xA5:
		AND(reg.l);
		return 4;

	//AND (HL)
	case 0xA6:
		AND(mem.read(reg.hl));
		return 8;

	//AND A
	case 0xA7:
		AND(reg.a);
		return 4;

	//XOR B
	case 0xA8:
		XOR(reg.b);
		return 4;

	//XOR C
	case 0xA9:
		XOR(reg.c);
		return 4;

	//XOR D
	case 0xAA:
		XOR(reg.d);
		return 4;

	//XOR E
	case 0xAB:
		XOR(reg.e);
		return 4;

	//XOR H
	case 0xAC:
		XOR(reg.h);
		return 4;

	//XOR L
	case 0xAD:
		XOR(reg.l);
		return 4;

	//XOR (HL)
	case 0xAE:
		XOR(mem.read(reg.hl));
		return 8;

	//XOR A
	case 0xAF:
		XOR(reg.a);
		return 4;

	//OR B
	case 0xB0:
		OR(reg.b);
		return 4;

	//OR C
	case 0xB1:
		OR(reg.c);
		return 4;

	//OR D
	case 0xB2:
		OR(reg.d);
		return 4;

	//OR E
	case 0xB3:
		OR(reg.e);
		return 4;

	//OR H
	case 0xB4:
		OR(reg.h);
		return 4;

	//OR L
	case 0xB5:
		OR(reg.l);
		return 4;

	//OR (HL)
	case 0xB6:
		OR(mem.read(reg.hl));
		return 8;

	//OR A
	case 0xB7:
		OR(reg.a);
		return 4;

	//CP B
	case 0xB8:
		CP(reg.b);
		return 4;

	//CP C
	case 0xB9:
		CP(reg.c);
		return 4;

	//CP D
	case 0xBA:
		CP(reg.d);
		return 4;

	//CP E
	case 0xBB:
		CP(reg.e);
		return 4;

	//CP H
	case 0xBC:
		CP(reg.h);
		return 4;

	//CP L
	case 0xBD:
		CP(reg.l);
		return 4;

	//CP (HL)
	case 0xBE:
		CP(mem.read(reg.hl));
		return 8;

	//CP A
	case 0xBF:
		CP(reg.a);
		return 4;

	//RET NZ
	case 0xC0:
		if (reg.getFlag(ZFLAG)) {
			return 8;
		}
		else {
			RET();
			return 20;
		}

	//POP BC
	case 0xC1:
		reg.c = mem.read(reg.sp);
		reg.b = mem.read(++reg.sp);
		reg.sp++;
		return 12;

	//JP NZ, a16
	case 0xC2:
		if (reg.getFlag(ZFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}

	//JP a16
	case 0xC3:
		JP(mem.read(reg.pc), mem.read(reg.pc + 1));
		return 16;

	//CALL NZ, a16
	case 0xC4:
		if (reg.getFlag(ZFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}

	//PUSH BC
	case 0xC5:
		reg.sp--;
		mem.write(reg.sp, reg.b);
		reg.sp--;
		mem.write(reg.sp, reg.c);
		return 16;

	//ADD A, d8
	case 0xC6:
		ADD_8bit(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 00H
	case 0xC7:
		RST(0x00);
		return 16;

	//RET Z
	case 0xC8:
		if (reg.getFlag(ZFLAG)) {
			RET();
			return 20;
		}
		else {
			return 8;
		}

	//RET
	case 0xC9:
		RET();
		return 16;
	
	//JP Z, a16
	case 0xCA:
		if (reg.getFlag(ZFLAG)) {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}
		else {
			reg.pc += 2;
			return 12;
		}

	//CB Prefix
	case 0xCB:
		return executeCB(mem.read(reg.pc));

	//CALL Z, a16
	case 0xCC:
		if (reg.getFlag(ZFLAG)) {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}
		else {
			reg.pc += 2;
			return 12;
		}

	//CALL a16
	case 0xCD:
		CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
		return 24;

	//ADC A, d8
	case 0xCE:
		ADC(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 08H
	case 0xCF:
		RST(0x08);
		return 16;

	//RET NC
	case 0xD0:
		if (reg.getFlag(CFLAG)) {
			return 8;
		}
		else {
			RET();
			return 20;
		}

	//POP DE
	case 0xD1:
		reg.e = mem.read(reg.sp);
		reg.d = mem.read(++reg.sp);
		reg.sp++;
		return 12;

	//JP NC, a16
	case 0xD2:
		if (reg.getFlag(CFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}

	//CALL NC, a16
	case 0xD4:
		if (reg.getFlag(CFLAG)) {
			reg.pc += 2;
			return 12;
		}
		else {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}

	//PUSH DE
	case 0xD5:
		mem.write(--reg.sp, reg.d);
		mem.write(--reg.sp, reg.e);
		return 16;

	//SUB d8
	case 0xD6:
		SUB(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 10H
	case 0xD7:
		RST(0x10);
		return 16;

	//RET C
	case 0xD8:
		if (reg.getFlag(CFLAG)) {
			RET();
			return 20;
		}
		else {
			return 8;
		}

	//RETI
	case 0xD9:
		RETI();
		return 16;

	//JP C, a16
	case 0xDA:
		if (reg.getFlag(CFLAG)) {
			JP(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 16;
		}
		else {
			reg.pc += 2;
			return 12;
		}

	//CALL C, a16
	case 0xDC:
		if (reg.getFlag(CFLAG)) {
			CALL(mem.read(reg.pc), mem.read(reg.pc + 1));
			return 24;
		}
		else {
			reg.pc += 2;
			return 12;
		}

	//SBC A, d8
	case 0xDE:
		SBC(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 18H
	case 0xDF:
		RST(0x18);
		return 16;

	//LD (0xFF00+a8), A
	case 0xE0:
		mem.write((0xFF00 + mem.read(reg.pc)), reg.a);
		reg.pc++;
		return 12;

	//POP HL
	case 0xE1:
		reg.l = mem.read(reg.sp);
		reg.h = mem.read(++reg.sp);
		reg.sp++;
		return 12;

	//LD (C), A
	case 0xE2:
		mem.write((0xFF00 + reg.c), reg.a);
		return 8;

	//PUSH HL
	case 0xE5:
		mem.write(--reg.sp, reg.h);
		mem.write(--reg.sp, reg.l);
		return 16;

	//AND d8
	case 0xE6:
		AND(mem.read(reg.pc));
		reg.pc++;
		return 8;
	
	//RST 20H
	case 0xE7:
		RST(0x20);
		return 16;
	
	//ADD SP, r8
	case 0xE8:
		ADD_sp((signed_byte)mem.read(reg.pc));
		reg.pc++;
		return 16;

	//JP (HL)
	case 0xE9:
		JP(reg.l, reg.h);
		return 4;

	//LD (a16), A
	case 0xEA: ///byte reversal
		mem.write((word)(mem.read(reg.pc + 1) << 8) | mem.read(reg.pc), reg.a);
		reg.pc += 2;
		return 16;

	//XOR d8
	case 0xEE:
		XOR(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 28H
	case 0xEF:
		RST(0x28);
		return 16;

	//LD A, (0xFF00+a8)
	case 0xF0:
		reg.a = mem.read((0xFF00 + mem.read(reg.pc)));
		reg.pc++;
		return 12;

	//POP AF
	case 0xF1:
		reg.f = mem.read(reg.sp);
		reg.f &= 0xF0;
		reg.a = mem.read(++reg.sp);
		reg.sp++;
		return 12;

	//LD A, (C)
	case 0xF2:
		reg.a = mem.read((0xFF00 + reg.c));
		return 8;

	//DI
	case 0xF3:
		IME = 0;
		return 4;

	//PUSH AF
	case 0xF5:
		mem.write(--reg.sp, reg.a);
		mem.write(--reg.sp, reg.f);
		return 16;

	//OR d8
	case 0xF6:
		OR(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 30H
	case 0xF7:
		RST(0x30);
		return 16;

	//LD HL, SP+r8
	case 0xF8:
		LDHL((signed_byte)mem.read(reg.pc));
		reg.pc++;
		return 12;

	//LD SP, HL
	case 0xF9:
		reg.sp = reg.hl;
		return 8;

	//LD A, (a16)
	case 0xFA:
		reg.a = mem.read((mem.read(reg.pc + 1) << 8) | mem.read(reg.pc));
		reg.pc += 2;
		return 16;

	//EI
	case 0xFB:
		IME = 1;
		return 4;

	//CP d8
	case 0xFE:
		CP(mem.read(reg.pc));
		reg.pc++;
		return 8;

	//RST 38H
	case 0xFF:
		RST(0x38);
		return 16;


	}

}

byte CPU::executeCB(byte opcode)
{
	reg.pc++;

	switch (opcode) {

	//RLC B
	case 0x00:
		reg.b = RLC(reg.b);
		return 8;

	//RLC C
	case 0x01:
		reg.c = RLC(reg.c);
		return 8;
	
	//RLC D
	case 0x02:
		reg.d = RLC(reg.d);
		return 8;

	//RLC E
	case 0x03:
		reg.e = RLC(reg.e);
		return 8;

	//RLC H
	case 0x04:
		reg.h = RLC(reg.h);
		return 8;

	//RLC L
	case 0x05:
		reg.l = RLC(reg.l);
		return 8;

	//RLC (HL)
	case 0x06:
		mem.write(reg.hl, RLC(mem.read(reg.hl)));
		return 16;

	//RLC A
	case 0x07:
		reg.a = RLC(reg.a);
		return 8;

	//RRC B
	case 0x08:
		reg.b = RRC(reg.b);
		return 8;

	//RRC C
	case 0x09:
		reg.c = RRC(reg.c);
		return 8;

	//RRC D
	case 0x0A:
		reg.d = RRC(reg.d);
		return 8;

	//RRC E
	case 0x0B:
		reg.e = RRC(reg.e);
		return 8;

	//RRC H
	case 0x0C:
		reg.h = RRC(reg.h);
		return 8;

	//RRC L
	case 0x0D:
		reg.l = RRC(reg.l);
		return 8;

	//RRC (HL)
	case 0x0E:
		mem.write(reg.hl, RRC(mem.read(reg.hl)));
		return 16;

	//RRC A
	case 0x0F:
		reg.a = RRC(reg.a);
		return 8;

	//RL B
	case 0x10:
		reg.b = RL(reg.b);
		return 8;

	//RL C
	case 0x11:
		reg.c = RL(reg.c);
		return 8;

	//RL D
	case 0x12:
		reg.d = RL(reg.d);
		return 8;

	//RL E
	case 0x13:
		reg.e = RL(reg.e);
		return 8;

	//RL H
	case 0x14:
		reg.h = RL(reg.h);
		return 8;

	//RL L
	case 0x15:
		reg.l = RL(reg.l);
		return 8;

	//RL (HL)
	case 0x16:
		mem.write(reg.hl, RL(mem.read(reg.hl)));
		return 16;

	//RL A
	case 0x17:
		reg.a = RL(reg.a);
		return 8;

	//RR B
	case 0x18:
		reg.b = RR(reg.b);
		return 8;

	//RR C
	case 0x19:
		reg.c = RR(reg.c);
		return 8;

	//RR D
	case 0x1A:
		reg.d = RR(reg.d);
		return 8;

	//RR E
	case 0x1B:
		reg.e = RR(reg.e);
		return 8;

	//RR H
	case 0x1C:
		reg.h = RR(reg.h);
		return 8;

	//RR L
	case 0x1D:
		reg.l = RR(reg.l);
		return 8;

	//RR (HL)
	case 0x1E:
		mem.write(reg.hl, RR(mem.read(reg.hl)));
		return 16;

	//RR A
	case 0x1F:
		reg.a = RR(reg.a);
		return 8;

	//SLA B
	case 0x20:
		reg.b = SLA(reg.b);
		return 8;

	//SLA C
	case 0x21:
		reg.c = SLA(reg.c);
		return 8;

	//SLA D
	case 0x22:
		reg.d = SLA(reg.d);
		return 8;

	//SLA E
	case 0x23:
		reg.e = SLA(reg.e);
		return 8;

	//SLA H
	case 0x24:
		reg.h = SLA(reg.h);
		return 8;

	//SLA L
	case 0x25:
		reg.l = SLA(reg.l);
		return 8;

	//SLA (HL)
	case 0x26:
		mem.write(reg.hl, SLA(mem.read(reg.hl)));
		return 16;

	//SLA A
	case 0x27:
		reg.a = SLA(reg.a);
		return 8;

	//SRA B
	case 0x28:
		reg.b = SRA(reg.b);
		return 8;

	//SRA C
	case 0x29:
		reg.c = SRA(reg.c);
		return 8;

	//SRA D
	case 0x2A:
		reg.d = SRA(reg.d);
		return 8;

	//SRA E
	case 0x2B:
		reg.e = SRA(reg.e);
		return 8;

	//SRA H
	case 0x2C:
		reg.h = SRA(reg.h);
		return 8;

	//SRA L
	case 0x2D:
		reg.l = SRA(reg.l);
		return 8;

	//SRA (HL)
	case 0x2E:
		mem.write(reg.hl, SRA(mem.read(reg.hl)));
		return 16;

	//SRA A
	case 0x2F:
		reg.a = SRA(reg.a);
		return 8;

	//SWAP B
	case 0x30:
		reg.b = SWAP(reg.b);
		return 8;

	//SWAP C
	case 0x31:
		reg.c = SWAP(reg.c);
		return 8;

	//SWAP D
	case 0x32:
		reg.d = SWAP(reg.d);
		return 8;

	//SWAP E
	case 0x33:
		reg.e = SWAP(reg.e);
		return 8;

	//SWAP H
	case 0x34:
		reg.h = SWAP(reg.h);
		return 8;

	//SWAP L
	case 0x35:
		reg.l = SWAP(reg.l);
		return 8;

	//SWAP (HL)
	case 0x36:
		mem.write(reg.hl, SWAP(mem.read(reg.hl)));
		return 16;

	//SWAP A
	case 0x37:
		reg.a = SWAP(reg.a);
		return 8;

	//SRL B
	case 0x38:
		reg.b = SRL(reg.b);
		return 8;

	//SRL C
	case 0x39:
		reg.c = SRL(reg.c);
		return 8;

	//SRL D
	case 0x3A:
		reg.d = SRL(reg.d);
		return 8;

	//SRL E
	case 0x3B:
		reg.e = SRL(reg.e);
		return 8;

	//SRL H
	case 0x3C:
		reg.h = SRL(reg.h);
		return 8;

	//SRL L
	case 0x3D:
		reg.l = SRL(reg.l);
		return 8;

	//SRL (HL)
	case 0x3E:
		mem.write(reg.hl, SRL(mem.read(reg.hl)));
		return 16;

	//SRL A
	case 0x3F:
		reg.a = SRL(reg.a);
		return 8;

	//BIT 0, B
	case 0x40:
		BIT(reg.b, 0x01);
		return 8;

	//BIT 0, C
	case 0x41:
		BIT(reg.c, 0x01);
		return 8;

	//BIT 0, D
	case 0x42:
		BIT(reg.d, 0x01);
		return 8;

	//BIT 0, E
	case 0x43:
		BIT(reg.e, 0x01);
		return 8;

	//BIT 0, H
	case 0x44:
		BIT(reg.h, 0x01);
		return 8;

	//BIT 0, L
	case 0x45:
		BIT(reg.l, 0x01);
		return 8;

	//BIT 0, (HL)
	case 0x46:
		BIT(mem.read(reg.hl), 0x01);
		return 12;

	//BIT 0, A
	case 0x47:
		BIT(reg.a, 0x01);
		return 8;

	//BIT 1, B
	case 0x48:
		BIT(reg.b, 0x02);
		return 8;

	//BIT 1, C
	case 0x49:
		BIT(reg.c, 0x02);
		return 8;

	//BIT 1, D
	case 0x4A:
		BIT(reg.d, 0x02);
		return 8;

	//BIT 1, E
	case 0x4B:
		BIT(reg.e, 0x02);
		return 8;

	//BIT 1, H
	case 0x4C:
		BIT(reg.h, 0x02);
		return 8;

	//BIT 1, L
	case 0x4D:
		BIT(reg.l, 0x02);
		return 8;

	//BIT 1, (HL)
	case 0x4E:
		BIT(mem.read(reg.hl), 0x02);
		return 12;

	//BIT 1, A
	case 0x4F:
		BIT(reg.a, 0x02);
		return 8;

	//BIT 2, B
	case 0x50:
		BIT(reg.b, 0x04);
		return 8;

	//BIT 2, C
	case 0x51:
		BIT(reg.c, 0x04);
		return 8;

	//BIT 2, D
	case 0x52:
		BIT(reg.d, 0x04);
		return 8;

	//BIT 2, E
	case 0x53:
		BIT(reg.e, 0x04);
		return 8;

	//BIT 2, H
	case 0x54:
		BIT(reg.h, 0x04);
		return 8;

	//BIT 2, L
	case 0x55:
		BIT(reg.l, 0x04);
		return 8;

	//BIT 2, (HL)
	case 0x56:
		BIT(mem.read(reg.hl), 0x04);
		return 12;
	
	//BIT 2, A
	case 0x57:
		BIT(reg.a, 0x04);
		return 8;

	//BIT 3, B
	case 0x58:
		BIT(reg.b, 0x08);
		return 8;

	//BIT 3, C
	case 0x59:
		BIT(reg.c, 0x08);
		return 8;

	//BIT 3, D
	case 0x5A:
		BIT(reg.d, 0x08);
		return 8;

	//BIT 3, E
	case 0x5B:
		BIT(reg.e, 0x08);
		return 8;

	//BIT 3, H
	case 0x5C:
		BIT(reg.h, 0x08);
		return 8;

	//BIT 3, L
	case 0x5D:
		BIT(reg.l, 0x08);
		return 8;
	
	//BIT 3, (HL)
	case 0x5E:
		BIT(mem.read(reg.hl), 0x08);
		return 12;

	//BIT 3, A
	case 0x5F:
		BIT(reg.a, 0x08);
		return 8;

	//BIT 4, B
	case 0x60:
		BIT(reg.b, 0x10);
		return 8;

	//BIT 4, C
	case 0x61:
		BIT(reg.c, 0x10);
		return 8;

	//BIT 4, D
	case 0x62:
		BIT(reg.d, 0x10);
		return 8;

	//BIT 4, E
	case 0x63:
		BIT(reg.e, 0x10);
		return 8;

	//BIT 4, H
	case 0x64:
		BIT(reg.h, 0x10);
		return 8;

	//BIT 4, L
	case 0x65:
		BIT(reg.l, 0x10);
		return 8;

	//BIT 4, (HL)
	case 0x66:
		BIT(mem.read(reg.hl), 0x10);
		return 12;

	//BIT 4, A
	case 0x67:
		BIT(reg.a, 0x10);
		return 8;

	//BIT 5, B
	case 0x68:
		BIT(reg.b, 0x20);
		return 8;

	//BIT 5, C
	case 0x69:
		BIT(reg.c, 0x20);
		return 8;

	//BIT 5, D
	case 0x6A:
		BIT(reg.d, 0x20);
		return 8;

	//BIT 5, E
	case 0x6B:
		BIT(reg.e, 0x20);
		return 8;

	//BIT 5, H
	case 0x6C:
		BIT(reg.h, 0x20);
		return 8;

	//BIT 5, L
	case 0x6D:
		BIT(reg.l, 0x20);
		return 8;

	//BIT 5, (HL)
	case 0x6E:
		BIT(mem.read(reg.hl), 0x20);
		return 12;

	//BIT 5, A
	case 0x6F:
		BIT(reg.a, 0x20);
		return 8;

	//BIT 6, B
	case 0x70:
		BIT(reg.b, 0x40);
		return 8;

	//BIT 6, C
	case 0x71:
		BIT(reg.c, 0x40);
		return 8;

	//BIT 6, D
	case 0x72:
		BIT(reg.d, 0x40);
		return 8;

	//BIT 6, E
	case 0x73:
		BIT(reg.e, 0x40);
		return 8;

	//BIT 6, H
	case 0x74:
		BIT(reg.h, 0x40);
		return 8;

	//BIT 6, L
	case 0x75:
		BIT(reg.l, 0x40);
		return 8;

	//BIT 6, (HL)
	case 0x76:
		BIT(mem.read(reg.hl), 0x40);
		return 12;

	//BIT 6, A
	case 0x77:
		BIT(reg.a, 0x40);
		return 8;

	//BIT 7, B
	case 0x78:
		BIT(reg.b, 0x80);
		return 8;

	//BIT 7, C
	case 0x79:
		BIT(reg.c, 0x80);
		return 8;

	//BIT 7, D
	case 0x7A:
		BIT(reg.d, 0x80);
		return 8;

	//BIT 7, E
	case 0x7B:
		BIT(reg.e, 0x80);
		return 8;

	//BIT 7, H
	case 0x7C:
		BIT(reg.h, 0x80);
		return 8;

	//BIT 7, L
	case 0x7D:
		BIT(reg.l, 0x80);
		return 8;

	//BIT 7, (HL)
	case 0x7E:
		BIT(mem.read(reg.hl), 0x80);
		return 12;

	//BIT 7, A
	case 0x7F:
		BIT(reg.a, 0x80);
		return 8;

	//RES 0, B
	case 0x80:
		reg.b &= ~(0x01);
		return 8;

	//RES 0, C
	case 0x81:
		reg.c &= ~(0x01);
		return 8;

	//RES 0, D
	case 0x82:
		reg.d &= ~(0x01);
		return 8;

	//RES 0, E
	case 0x83:
		reg.e &= ~(0x01);
		return 8;

	//RES 0, H
	case 0x84:
		reg.h &= ~(0x01);
		return 8;

	//RES 0, L
	case 0x85:
		reg.l &= ~(0x01);
		return 8;

	//RES 0, (HL)
	case 0x86:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x01));
		return 16;

	//RES 0, A
	case 0x87:
		reg.a &= ~(0x01);
		return 8;

	//RES 1, B
	case 0x88:
		reg.b &= ~(0x02);
		return 8;

	//RES 1, C
	case 0x89:
		reg.c &= ~(0x02);
		return 8;

	//RES 1, D
	case 0x8A:
		reg.d &= ~(0x02);
		return 8;

	//RES 1, E
	case 0x8B:
		reg.e &= ~(0x02);
		return 8;

	//RES 1, H
	case 0x8C:
		reg.h &= ~(0x02);
		return 8;

	//RES 1, L
	case 0x8D:
		reg.l &= ~(0x02);
		return 8;

	//RES 1, (HL)
	case 0x8E:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x02));
		return 16;

	//RES 1, A
	case 0x8F:
		reg.a &= ~(0x02);
		return 8;

	//RES 2, B
	case 0x90:
		reg.b &= ~(0x04);
		return 8;

	//RES 2, C
	case 0x91:
		reg.c &= ~(0x04);
		return 8;

	//RES 2, D
	case 0x92:
		reg.d &= ~(0x04);
		return 8;

	//RES 2, E
	case 0x93:
		reg.e &= ~(0x04);
		return 8;

	//RES 2, H
	case 0x94:
		reg.h &= ~(0x04);
		return 8;

	//RES 2, L
	case 0x95:
		reg.l &= ~(0x04);
		return 8;

	//RES 2, (HL)
	case 0x96:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x04));
		return 16;

	//RES 2, A
	case 0x97:
		reg.a &= ~(0x04);
		return 8;

	//RES 3, B
	case 0x98:
		reg.b &= ~(0x08);
		return 8;

	//RES 3, C
	case 0x99:
		reg.c &= ~(0x08);
		return 8;

	//RES 3, D
	case 0x9A:
		reg.d &= ~(0x08);
		return 8;

	//RES 3, E
	case 0x9B:
		reg.e &= ~(0x08);
		return 8;

	//RES 3, H
	case 0x9C:
		reg.h &= ~(0x08);
		return 8;

	//RES 3, L
	case 0x9D:
		reg.l &= ~(0x08);
		return 8;

	//RES 3, (HL)
	case 0x9E:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x08));
		return 16;

	//RES 3, A
	case 0x9F:
		reg.a &= ~(0x08);
		return 8;

	//RES 4, B
	case 0xA0:
		reg.b &= ~(0x10);
		return 8;

	//RES 4, C
	case 0xA1:
		reg.c &= ~(0x10);
		return 8;

	//RES 4, D
	case 0xA2:
		reg.d &= ~(0x10);
		return 8;

	//RES 4, E
	case 0xA3:
		reg.e &= ~(0x10);
		return 8;

	//RES 4, H
	case 0xA4:
		reg.h &= ~(0x10);
		return 8;

	//RES 4, L
	case 0xA5:
		reg.l &= ~(0x10);
		return 8;

	//RES 4, (HL)
	case 0xA6:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x10));
		return 16;

	//RES 4, A
	case 0xA7:
		reg.a &= ~(0x10);
		return 8;

	//RES 5, B
	case 0xA8:
		reg.b &= ~(0x20);
		return 8;

	//RES 5, C
	case 0xA9:
		reg.c &= ~(0x20);
		return 8;

	//RES 5, D
	case 0xAA:
		reg.d &= ~(0x20);
		return 8;

	//RES 5, E
	case 0xAB:
		reg.e &= ~(0x20);
		return 8;

	//RES 5, H
	case 0xAC:
		reg.h &= ~(0x20);
		return 8;

	//RES 5, L
	case 0xAD:
		reg.l &= ~(0x20);
		return 8;

	//RES 5, (HL)
	case 0xAE:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x20));
		return 16;

	//RES 5, A
	case 0xAF:
		reg.a &= ~(0x20);
		return 8;

	//RES 6, B
	case 0xB0:
		reg.b &= ~(0x40);
		return 8;

	//RES 6, C
	case 0xB1:
		reg.c &= ~(0x40);
		return 8;

	//RES 6, D
	case 0xB2:
		reg.d &= ~(0x40);
		return 8;

	//RES 6, E
	case 0xB3:
		reg.e &= ~(0x40);
		return 8;

	//RES 6, H
	case 0xB4:
		reg.h &= ~(0x40);
		return 8;

	//RES 6, L
	case 0xB5:
		reg.l &= ~(0x40);
		return 8;

	//RES 6, (HL)
	case 0xB6:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x40));
		return 16;

	//RES 6, A
	case 0xB7:
		reg.a &= ~(0x40);
		return 8;

	//RES 7, B
	case 0xB8:
		reg.b &= ~(0x80);
		return 8;

	//RES 7, C
	case 0xB9:
		reg.c &= ~(0x80);
		return 8;

	//RES 7, D
	case 0xBA:
		reg.d &= ~(0x80);
		return 8;

	//RES 7, E
	case 0xBB:
		reg.e &= ~(0x80);
		return 8;

	//RES 7, H
	case 0xBC:
		reg.h &= ~(0x80);
		return 8;

	//RES 7, L
	case 0xBD:
		reg.l &= ~(0x80);
		return 8;

	//RES 7, (HL)
	case 0xBE:
		mem.write(reg.hl, mem.read(reg.hl) & ~(0x80));
		return 16;

	//RES 7, A
	case 0xBF:
		reg.a &= ~(0x80);
		return 8;

	//SET 0, B
	case 0xC0:
		reg.b |= 0x01;
		return 8;

	//SET 0, C
	case 0xC1:
		reg.c |= 0x01;
		return 8;

	//SET 0, D
	case 0xC2:
		reg.d |= 0x01;
		return 8;

	//SET 0, E
	case 0xC3:
		reg.e |= 0x01;
		return 8;

	//SET 0, H
	case 0xC4:
		reg.h |= 0x01;
		return 8;

	//SET 0, L
	case 0xC5:
		reg.l |= 0x01;
		return 8;

	//SET 0, (HL)
	case 0xC6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x01);
		return 16;

	//SET 0, A
	case 0xC7:
		reg.a |= 0x01;
		return 8;

	//SET 1, B
	case 0xC8:
		reg.b |= 0x02;
		return 8;

	//SET 1, C
	case 0xC9:
		reg.c |= 0x02;
		return 8;

	//SET 1, D
	case 0xCA:
		reg.d |= 0x02;
		return 8;

	//SET 1, E
	case 0xCB:
		reg.e |= 0x02;
		return 8;

	//SET 1, H
	case 0xCC:
		reg.h |= 0x02;
		return 8;

	//SET 1, L
	case 0xCD:
		reg.l |= 0x02;
		return 8;

	//SET 1, (HL)
	case 0xCE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x02);
		return 16;

	//SET 1, A
	case 0xCF:
		reg.a |= 0x02;
		return 8;

	//SET 2, B
	case 0xD0:
		reg.b |= 0x04;
		return 8;

	//SET 2, C
	case 0xD1:
		reg.c |= 0x04;
		return 8;

	//SET 2, D
	case 0xD2:
		reg.d |= 0x04;
		return 8;

	//SET 2, E
	case 0xD3:
		reg.e |= 0x04;
		return 8;

	//SET 2, H
	case 0xD4:
		reg.h |= 0x04;
		return 8;

	//SET 2, L
	case 0xD5:
		reg.l |= 0x04;
		return 8;

	//SET 2, (HL)
	case 0xD6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x04);
		return 16;

	//SET 2, A
	case 0xD7:
		reg.a |= 0x04;
		return 8;

	//SET 3, B
	case 0xD8:
		reg.b |= 0x08;
		return 8;

	//SET 3, C
	case 0xD9:
		reg.c |= 0x08;
		return 8;

	//SET 3, D
	case 0xDA:
		reg.d |= 0x08;
		return 8;

	//SET 3, E
	case 0xDB:
		reg.e |= 0x08;
		return 8;

	//SET 3, H
	case 0xDC:
		reg.h |= 0x08;
		return 8;

	//SET 3, L
	case 0xDD:
		reg.l |= 0x08;
		return 8;

	//SET 3, (HL)
	case 0xDE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x08);
		return 16;

	//SET 3, A
	case 0xDF:
		reg.a |= 0x08;
		return 8;

	//SET 4, B
	case 0xE0:
		reg.b |= 0x10;
		return 8;

	//SET 4, C
	case 0xE1:
		reg.c |= 0x10;
		return 8;

	//SET 4, D
	case 0xE2:
		reg.d |= 0x10;
		return 8;

	//SET 4, E
	case 0xE3:
		reg.e |= 0x10;
		return 8;

	//SET 4, H
	case 0xE4:
		reg.h |= 0x10;
		return 8;

	//SET 4, L
	case 0xE5:
		reg.l |= 0x10;
		return 8;

	//SET 4, (HL)
	case 0xE6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x10);
		return 16;

	//SET 4, A
	case 0xE7:
		reg.a |= 0x10;
		return 8;

	//SET 5, B
	case 0xE8:
		reg.b |= 0x20;
		return 8;

	//SET 5, C
	case 0xE9:
		reg.c |= 0x20;
		return 8;

	//SET 5, D
	case 0xEA:
		reg.d |= 0x20;
		return 8;

	//SET 5, E
	case 0xEB:
		reg.e |= 0x20;
		return 8;

	//SET 5, H
	case 0xEC:
		reg.h |= 0x20;
		return 8;

	//SET 5, L
	case 0xED:
		reg.l |= 0x20;
		return 8;

	//SET 5, (HL)
	case 0xEE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x20);
		return 16;

	//SET 5, A
	case 0xEF:
		reg.a |= 0x20;
		return 8;

	//SET 6, B
	case 0xF0:
		reg.b |= 0x40;
		return 8;

	//SET 6, C
	case 0xF1:
		reg.c |= 0x40;
		return 8;

	//SET 6, D
	case 0xF2:
		reg.d |= 0x40;
		return 8;

	//SET 6, E
	case 0xF3:
		reg.e |= 0x40;
		return 8;

	//SET 6, H
	case 0xF4:
		reg.h |= 0x40;
		return 8;

	//SET 6, L
	case 0xF5:
		reg.l |= 0x40;
		return 8;

	//SET 6, (HL)
	case 0xF6:
		mem.write(reg.hl, mem.read(reg.hl) | 0x40);
		return 16;

	//SET 6, A
	case 0xF7:
		reg.a |= 0x40;
		return 8;

	//SET 7, B
	case 0xF8:
		reg.b |= 0x80;
		return 8;

	//SET 7, C
	case 0xF9:
		reg.c |= 0x80;
		return 8;

	//SET 7, D
	case 0xFA:
		reg.d |= 0x80;
		return 8;

	//SET 7, E
	case 0xFB:
		reg.e |= 0x80;
		return 8;

	//SET 7, H
	case 0xFC:
		reg.h |= 0x80;
		return 8;

	//SET 7, L
	case 0xFD:
		reg.l |= 0x80;
		return 8;

	//SET 7, (HL)
	case 0xFE:
		mem.write(reg.hl, mem.read(reg.hl) | 0x80);
		return 16;

	//SET 7, A
	case 0xFF:
		reg.a |= 0x80;
		return 8;

	}

}