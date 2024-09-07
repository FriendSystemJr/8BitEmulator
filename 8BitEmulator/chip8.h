#pragma once

class Chip8 {
public:

	void initialize();
	void loadGame(const char* game);
	void emulateCycle();
	bool drawFlag();

private:
	//Variables

	//stores the (current?) opcode
	unsigned short m_opcode;

	/*
	0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
	0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
	0x200-0xFFF - Program ROM and work RAM
	*/

	//Memory of the chip
	unsigned char m_memory[4096];

	//Registers
	//Chip 8 has 15 general purpose registers (from V0 to VE)
	//16th register is used for a carry flag
	unsigned char m_V[16];

	//Both registers go from 0x000 to 0xFFF
	//Index register
	unsigned short m_I;

	//Program counter
	unsigned short m_pc;

	//Graphics
	unsigned char m_gfx[64 * 32];

	//Timers
	//Registers that count at 60 hz. When set above zero they count down to zero
	unsigned char m_delay_timer;
	unsigned char m_sound_timer;


	/*There are opcodes for jumping to a certain address or subroutine.
	The stack is used to remember the current location before the jump
	The system has 16 levels of stack so the Stack ptr is used to remember which level of the stack is used
	*/

	//Stack
	unsigned short m_stack[16];
	//Stack ptr
	unsigned short m_sp;

	//HEX-based keypad (0x0-0xF)
	unsigned char m_key[16];
};