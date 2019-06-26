#include <fstream>

std::map<std::string, std::tuple<int, int, int>> instr = {
/*    ins    code  addr_mode    op_cnt */
    {"mov", {0x82, 0b0111'1111, 2       }},
    {"sti", {0x82, 0b0000'0000, 0       }},
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
		[-] get opts and split file in lines
		[-] make an example code file
		[-] populate instr metadata
		[-] parse instr:
			[-] decode direction
			[-] decode mode
			[-] decode registers involved based on mode
			[-] decode const involved (opt: hex, bin, dec, ?oct?)
		[-] compute label address if necesary
*/

int main(int argc, char const *argv[])
{
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