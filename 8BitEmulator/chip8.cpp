#include "chip8.h"

#include <fstream>
#include <random>

unsigned int randomNumber();

Chip8::Chip8(Renderer::Grid& grid) : m_grid(grid) {
	drawFlag = false;
}

const unsigned char(&Chip8::GetGFX() const)[64][32]{
	return m_gfx;
}

void Chip8::initialize() {
	m_pc = 0x200;	//application is loaded at this location
	m_opcode = 0;	//Reset current opcode
	m_I = 0;		//Reset index register
	m_sp = 0;		//Reset stack pointer

	//Clear display
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 32; ++j) {
			m_gfx[i][j] = 0;
		}
	}

	//Clear stack
	for (int i = 0; i < 16; ++i) {
		m_stack[i] = 0;
	}

	//Clear registers V0-VF
	for (int i = 0; i < 16; ++i) {
		m_V[i] = 0;
	}

	//Clear memory
	for (int i = 0; i < 4096; ++i) {
		m_memory[i] = 0;
	}

	//Clear timers
	m_sound_timer = 0;
	m_delay_timer = 0;

	//Load fontset
	for (int i = 0x050; i < 0x0A0; ++i) {
		m_memory[i] = m_fontset[i-0x050];
	}

}

void Chip8::loadGame(const char* game) {
	std::ifstream file(game, std::ios::binary);

	if (!file) {
		std::cerr << "Couldn't load file!\n";
		return;
	}

	//Read the file and cast unsigned char* to char, then specify max size for loading data
	file.read(reinterpret_cast<char*>(m_memory + 0x200), sizeof(m_memory) - 0x200);
	file.close();

	std::cout << "Read " << file.gcount() << " bytes!\n";

}

void Chip8::emulateCycle() {
	//Fetch opcode
	m_opcode = m_memory[m_pc] << 8 | m_memory[m_pc + 1];

	std::cout << "Instruction: " << std::hex << m_opcode << "\n";

	//Decode opcode (1x Opcode = 4x 4 bit; durch bit-AND alle bits 0 außer ersten 4 (höchster Nibble um Opcode-Kategorie zu filtern))
	switch (m_opcode & 0xF000) {
	case 0x0000:
		switch (m_opcode & 0x000F) {
		case 0x0000: //Clears the screen TODO: Rework for chip-8 logic
			m_grid.Clear();
			m_pc += 2;
			break;
		case 0x000E: //Return from subroutine
			--m_sp;
			m_pc = m_stack[m_sp];
			m_pc += 2;
			break;
		}
		break;
	case 0x1000: //Jumps to address NNN
		if (m_opcode == 0x13DC) {
			int a;
		}
		m_pc = m_opcode & 0x0FFF;
		break;
	case 0x2000: //Calls subroutine at NNN
		m_stack[m_sp] = m_pc;
		++m_sp;
		m_pc = m_opcode & 0x0FFF;
		break;
	case 0x3000: //Skips the next instruction if VX equals NN(usually the next instruction is a jump to skip a code block)
		if (m_V[(m_opcode & 0x0F00) >> 8] == (m_opcode & 0x00FF)) {
			m_pc += 4;
		}
		else {
			m_pc += 2;
		}
		break;
	case 0x4000: //Skips the next instruction if VX does not equal NN (usually the next instruction is a jump to skip a code block)
		if (m_V[(m_opcode & 0x0F00) >> 8] != (m_opcode & 0x00FF)) {
			m_pc += 4;
		}
		else {
			m_pc += 2;
		}
		break;
	case 0x5000: //Skips the next instruction if VX equals VY (usually the next instruction is a jump to skip a code block)
		if (m_V[(m_opcode & 0x0F00) >> 8] == m_V[(m_opcode & 0x00F0) >> 4]) {
			m_pc += 4;
		}
		else {
			m_pc += 2;
		}
		break;
	case 0x6000: //Sets VX to NN
		m_V[(m_opcode & 0x0F00) >> 8] = m_opcode & 0x00FF;
		m_pc += 2;
		break;
	case 0x7000: //Adds NN to VX (carry flag is not changed)
		m_V[(m_opcode & 0x0F00) >> 8] += m_opcode & 0x00FF;
		m_pc += 2;
		break;
	case 0x8000:
		switch (m_opcode & 0x000F) {
		case 0x0000: //Sets VX to the value of VY
			m_V[(m_opcode & 0x0F00) >> 8] = m_V[(m_opcode & 0x00F0) >> 4];
			m_pc += 2;
			break;
		case 0x0001: //Sets VX to VX or VY. (bitwise OR operation)
			m_V[(m_opcode & 0x0F00) >> 8] = m_V[(m_opcode & 0x0F00) >> 8] | m_V[(m_opcode & 0x00F0) >> 4];
			m_pc += 2;
			break;
		case 0x0002: //Sets VX to VX and VY. (bitwise AND operation)
			m_V[(m_opcode & 0x0F00) >> 8] = m_V[(m_opcode & 0x0F00) >> 8] & m_V[(m_opcode & 0x00F0) >> 4];
			m_pc += 2;
			break;
		case 0x0003: //Sets VX to VX xor VY
			m_V[(m_opcode & 0x0F00) >> 8] = m_V[(m_opcode & 0x0F00) >> 8] ^ m_V[(m_opcode & 0x00F0) >> 4];
			m_pc += 2;
			break;
		case 0x0004: //Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not
			m_V[(m_opcode & 0x0F00) >> 8] += m_V[(m_opcode & 0x00F0) >> 4];
			if (m_V[(m_opcode & 0x0F00) >> 8] < m_V[(m_opcode & 0x00F0) >> 4]) {
				m_V[15] = 1;
			}
			else {
				m_V[15] = 0;
			}
			m_pc += 2;
			break;
		case 0x0005: //VY is subtracted from VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VX >= VY and 0 if not)
			if (m_V[(m_opcode & 0x0F00) >> 8] < m_V[(m_opcode & 0x00F0) >> 4]) {
				m_V[15] = 0;
			}
			else {
				m_V[15] = 1;
			}
			m_V[(m_opcode & 0x0F00) >> 8] -= m_V[(m_opcode & 0x00F0) >> 4];

			m_pc += 2;
			break;
		case 0x0006: //Shifts VX to the right by 1, then stores the least significant bit of VX prior to the shift into VF
			m_V[15] = m_V[(m_opcode & 0x0F00) >> 8] & 0x1;
			m_V[(m_opcode & 0x0F00) >> 8] >>= 1;

			m_pc += 2;
			break;
		case 0x0007: //Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX)
			if (m_V[(m_opcode & 0x00F0) >> 4] < m_V[(m_opcode & 0x0F00) >> 8]) {
				m_V[15] = 0;
			}
			else {
				m_V[15] = 1;
			}
			m_V[(m_opcode & 0x0F00) >> 8] = m_V[(m_opcode & 0x00F0) >> 4] - m_V[(m_opcode & 0x0F00) >> 8];

			m_pc += 2;
			break;
		case 0x000E: //Shifts VX to the left by 1, then sets VF to 1 if the most significant bit of VX prior to that shift was set, or to 0 if it was unset
			m_V[15] = m_V[(m_opcode & 0x0F00) >> 8] & 0x1;
			m_V[(m_opcode & 0x0F00) >> 8] <<= 1;
			m_pc += 2;
			break;
		}
		break;
	case 0x9000: //Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block)
		if (m_V[(m_opcode & 0x0F00) >> 8] != m_V[(m_opcode & 0x00F0) >> 4]) {
			m_pc += 4;
		}
		else {
			m_pc += 2;
		}
		break;

	case 0xA000: //ANNN: Sets I to the address NNN
		m_I = m_opcode & 0x0FFF;
		m_pc += 2;
		break;

	case 0xB000: //Jumps to the address NNN plus V0
		m_pc = (m_opcode & 0x0FFF) + m_V[0];
		break;
	case 0xC000: //Sets VX to the result of a bitwise AND operation on a random number (Typically: 0 to 255) and NN
		m_V[(m_opcode & 0x0F00) >> 8] = (m_opcode & 0x00FF) & randomNumber();
		m_pc += 2;
		break;
	case 0xD000: //Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
		{

		std::cout << std::hex << m_opcode << "\n";

		unsigned short x = m_V[(m_opcode & 0x0F00) >> 8];
		unsigned short y = m_V[(m_opcode & 0x00F0) >> 4];
		unsigned short height = (m_opcode & 0x000F);
		unsigned short pixel;

		m_V[15] = 0;

		//Use the height to know how many values to read from I
		for (int yline = 0; yline < height; ++yline) {
			pixel = m_memory[m_I + yline];
			for (int xline = 0; xline < 8; ++xline) {
				// 0xFF for example in pixel
				//1111 1111
				//Check if current pixel is set (1) and if instruction is 1
				if((pixel & (0x80 >> xline)) != 0) {
					if (m_gfx[x + xline][y + yline] == 1) {
						m_V[15] = 1;
					}
					m_gfx[x + xline][y + yline] ^= 1;
					drawFlag = true;
				}
			}
		}
		
		
	}
		m_pc += 2;
		break;
	case 0xE000:
		switch (m_opcode & 0x00FF) {
		case 0x009E: //Skips the next instruction if the key stored in VX is pressed (usually the next instruction is a jump to skip a code block)
			//
			break;
		case 0x00A1: //Skips the next instruction if the key stored in VX is not pressed (usually the next instruction is a jump to skip a code block)
			//
			break;
		}
		break;
	case 0xF000:
		switch (m_opcode & 0x00FF) {
		case 0x0007: //Sets VX to the value of the delay timer
			m_V[(m_opcode & 0x0F00) >> 8] = m_delay_timer;
			m_pc += 2;
			break;
		case 0x000A: //A key press is awaited, and then stored in VX (blocking operation, all instruction halted until next key event)
			//
			break;
		case 0x0015: //Sets the delay timer to VX
			m_delay_timer = m_V[(m_opcode & 0x0F00) >> 8];
			m_pc += 2;
			break;
		case 0x0018: //Sets the sound timer to VX
			m_sound_timer = m_V[(m_opcode & 0x0F00) >> 8];
			m_pc += 2;
			break;
		case 0x001E: //Adds VX to I. VF is not affected
			m_I += m_V[(m_opcode & 0x0F00) >> 8];
			m_pc += 2;
			break;
		case 0x0029: //Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
			//
			m_pc += 2;
			break;
		case 0x0033: //Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2
			//
			m_pc += 2;
			break;
		case 0x0055: //Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
			for (int i = 0; i <= (m_opcode & 0x0F00) >> 8; ++i) {
				m_memory[m_I + i] = m_V[i];
			}
			m_pc += 2;
			break;
		case 0x0065: //Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified
			for (int i = 0; i <= (m_opcode & 0x0F00) >> 8; ++i) {
				m_V[i] = m_memory[m_I + i];
			}
			m_pc += 2;
			break;
		}
		break;

	default:
		std::cout << "Couldn't find opcode: "<< m_opcode <<"!\n";
	}

}

unsigned int randomNumber() {
	std::random_device seed;
	std::mt19937 gen{ seed() };

	std::uniform_int_distribution<> num{ 0, 255 };

	return num(gen);
}