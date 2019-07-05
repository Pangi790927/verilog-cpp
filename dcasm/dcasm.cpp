#include <fstream>
#include <iostream>
#include <string>
#include <map>

std::map<std::string, std::tuple<int, int, int>> instr = {
/*    ins      code  addr_mode    op_cnt */
    {"adc",   {0x01, 0b0111'1111, 2       }},
    {"add",   {0x02, 0b0111'1111, 2       }},
    {"cmp",   {0x03, 0b0111'1111, 2       }},
    {"dec",   {0x04, 0b0111'1111, 1       }},
    {"inc",   {0x05, 0b0111'1111, 1       }},
    {"neg",   {0x06, 0b0111'1111, 1       }},
    {"sbb",   {0x07, 0b0111'1111, 2       }},
    {"sub",   {0x08, 0b0111'1111, 2       }},
    
    {"and",   {0x10, 0b0111'1111, 2       }},
    {"not",   {0x11, 0b0111'1111, 1       }},
    {"or" ,   {0x12, 0b0111'1111, 2       }},
    {"xor",   {0x13, 0b0111'1111, 2       }},
    {"shl",   {0x14, 0b0111'1111, 1       }},
    {"shr",   {0x15, 0b0111'1111, 1       }},
    {"sal",   {0x16, 0b0111'1111, 1       }},
    {"sar",   {0x17, 0b0111'1111, 1       }},
    {"rol",   {0x18, 0b0111'1111, 1       }},
    {"ror",   {0x19, 0b0111'1111, 1       }},
    
    {"push",  {0x20, 0b0000'0001, 1       }},
    {"pop",   {0x21, 0b0000'0001, 1       }},
    {"pusha", {0x22, 0b0000'0001, 1       }},
    {"popa",  {0x23, 0b0000'0001, 1       }},
    {"pushf", {0x24, 0b0000'0001, 1       }},
    {"popf",  {0x25, 0b0000'0001, 1       }},
    
    {"call",  {0x30, 0b0111'1111, 1       }},
    {"int",   {0x31, 0b0111'1111, 1       }},
    {"iret",  {0x32, 0b0111'1111, 1       }},
    {"ja",    {0x33, 0b0111'1111, 1       }},
    {"jae",   {0x34, 0b0111'1111, 1       }},
    {"jb",    {0x35, 0b0111'1111, 1       }},
    {"jbe",   {0x36, 0b0111'1111, 1       }},
    {"jc",    {0x37, 0b0111'1111, 1       }},
    {"jcxz",  {0x38, 0b0111'1111, 1       }},
    {"je",    {0x39, 0b0111'1111, 1       }},
    {"jg",    {0x3A, 0b0111'1111, 1       }},
    {"jge",   {0x3B, 0b0111'1111, 1       }},
    {"jl",    {0x3C, 0b0111'1111, 1       }},
    {"jle",   {0x3D, 0b0111'1111, 1       }},
    {"jmp",   {0x3E, 0b0111'1111, 1       }},
    {"jna",   {0x3F, 0b0111'1111, 1       }},
    
    {"jnae",  {0x40, 0b0111'1111, 1       }},
    {"jnb",   {0x41, 0b0111'1111, 1       }},
    {"jnbe",  {0x42, 0b0111'1111, 1       }},
    {"jnc",   {0x43, 0b0111'1111, 1       }},
    {"jne",   {0x44, 0b0111'1111, 1       }},
    {"jng",   {0x45, 0b0111'1111, 1       }},
    {"jnge",  {0x46, 0b0111'1111, 1       }},
    {"jnl",   {0x47, 0b0111'1111, 1       }},
    {"jnle",  {0x48, 0b0111'1111, 1       }},
    {"jno",   {0x49, 0b0111'1111, 1       }},
    {"jnp",   {0x4A, 0b0111'1111, 1       }},
    {"jns",   {0x4B, 0b0111'1111, 1       }},
    {"jnz",   {0x4C, 0b0111'1111, 1       }},
    {"jo",    {0x4D, 0b0111'1111, 1       }},
    {"jp",    {0x4E, 0b0111'1111, 1       }},
    {"jpe",   {0x4F, 0b0111'1111, 1       }},
    
    {"jpo",   {0x50, 0b0111'1111, 1       }},
    {"js",    {0x51, 0b0111'1111, 1       }},
    {"jz",    {0x52, 0b0111'1111, 1       }},
    
    {"cmp",   {0x60, 0b0111'1111, 1       }},
    {"test",  {0x61, 0b0111'1111, 1       }},
    
    {"hlt",   {0x70, 0b0000'0000, 0       }},
    {"nop",   {0x71, 0b0000'0000, 0       }},
    
    {"in",    {0x80, 0b0111'1111, 2       }}, /* in  dest, port */
    {"out",   {0x81, 0b0111'1111, 2       }}, /* out port, dest */
    
    {"mov",   {0x90, 0b0111'1111, 2       }},
    {"xcgh",  {0x91, 0b0111'1111, 2       }},
    
    {"clc",   {0xA0, 0b0000'0000, 0       }},
    {"cld",   {0xA1, 0b0000'0000, 0       }},
    {"cli",   {0xA2, 0b0000'0000, 0       }},
    {"stc",   {0xA3, 0b0000'0000, 0       }},
    {"std",   {0xA4, 0b0000'0000, 0       }},
    {"sti",   {0xA5, 0b0000'0000, 0       }},
};

/*
	TO DO:
		* include files
		* more options
		* local labels
		* define directives
*/

/*
	TASK LIST:
		[+] get opts and split file in lines
		[+] populate instr metadata
		[-] compute label address if necesary
		[-] comments
		[-] parse instr:
			[-] decode direction
			[-] decode mode
			[-] decode registers involved based on mode
			[-] decode const involved (opt: hex, bin, dec, ?oct?)
		[-] make an example code file
*/

void parseLine(std::string &line) {
	std::cout << line << std::endl;
}

int parseLineByLine(std::ifstream &in) {
	if (!in.is_open()) {
		return 1;
	}

	std::string line;
	while (getline(in, line)) {
		parseLine(line);
    }

    return 0;
}

int main(int argc, char const *argv[]) {
	if (argc < 3) {
		std::cerr << "Syntax: dcasm <input_file.asm> <output_file.hex>" << std::endl;
		return 1;
	}

	std::ifstream in;
	in.open(argv[1]);

	if (parseLineByLine(in) != 0) {
		std::cerr << "Could not open input file" << std::endl;
		return 2;
	}

	in.close();

	std::ofstream out;
	out.open(argv[2]);

	if (!out.is_open()) {
		std::cerr << "Could not open output file" << std::endl;
		return 3;
	}

	out.close();

	return 0;
}

/*

instr
addr mode
label
indent registers

	EXAMPLE:
; %include "file" 
label:
	mov ax, bx
	mov ax, [bx]
	mov ax, [bx + 123]			; cop:mov | dir:1 | mod:5 | reg:0:1 | op1:1:1 - 123
	ADD ax, cx
	mov eax, [bx + 123]			; coments
	mov [bx + 123], eax			; coment 2
	jmp label
	.label_local:

	jmp .label_local

label2:

*/