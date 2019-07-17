#ifndef REGS_MAP_H
#define REGS_MAP_H

std::map<std::string, int> regs_map = {
	{ "eax", 0b00000 },
	{ "ax" , 0b00001 },
	{ "al" , 0b00010 },
	{ "ah" , 0b00011 },
	{ "ebx", 0b00100 },
	{ "bx" , 0b00101 },
	{ "bl" , 0b00110 },
	{ "bh" , 0b00111 },
	{ "ecx", 0b01000 },
	{ "cx" , 0b01001 },
	{ "cl" , 0b01010 },
	{ "ch" , 0b01011 },
	{ "edx", 0b01100 },
	{ "dx" , 0b01101 },
	{ "dl" , 0b01110 },
	{ "dh" , 0b01111 },
	{ "esi", 0b10000 },
	{ "edi", 0b10100 },
	{ "esp", 0b11000 },
	{ "ebp", 0b11100 },
};

#endif