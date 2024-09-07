#include "chip8.h"

void Chip8::initialize() {
	m_pc = 0x200;	//application is loaded at this location
	m_opcode = 0;	//Reset current opcode
	m_I = 0;		//Reset index register
	m_sp = 0;		//Reset stack pointer
}

void Chip8::emulateCycle() {
	//Fetch opcode
	//Decode opcode
	//Execute opcode
}