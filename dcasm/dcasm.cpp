#include <fstream>
#include <iostream>

#include "parser.h"

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
		[+] comments
		[-] parse instr:
			[-] decode direction
			[-] decode mode
			[-] decode registers involved based on mode
			[-] decode const involved (opt: hex, bin, dec, ?oct?)
		[+] make an example code file
*/

int main(int argc, char const *argv[]) {
	if (argc < 3) {
		std::cerr << "Syntax: dcasm <input_file.asm> <output_file.hex>" << std::endl;
		return 1;
	}

	try {
		Parser parser(argv[1], argv[2]);
		parser.parse();
	}
	catch (std::exception& err) {
		std::cout << "Exception while parsing: " << err.what() << std::endl;
	}

	return 0;
}
