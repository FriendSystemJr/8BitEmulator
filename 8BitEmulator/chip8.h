#pragma once
#include "Grid.h"

class Chip8 {
public:

	Chip8(Renderer::Grid& grid);

	void initialize();
	void loadGame(const char* game);
	void emulateCycle();
	bool drawFlag();

private:
	//Variables

	//Graphics
	Renderer::Grid& m_grid;

	//stores the (current?) opcode (2 bytes)
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

	//Fontset (Each number/character is 4 pixels wide and 5 pixels high)

	/*
	Explanation:
	0xF0 = 0b 1111 0000 -> **** (first line of the zero)
	0x90 = 0b 1001 0000 -> *  *
	0x90 = 0b 1001 0000 -> *  *
	0xF0 = 0b 1111 0000 -> ****
	*/
	unsigned char m_fontset[80] = {
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
};