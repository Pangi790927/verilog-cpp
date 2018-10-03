#include <iostream>
#include <verilated.h>
#include "obj_dir/Vcpu.h"

int main(int argc, char const *argv[])
{
	Vcpu cpu;
	cpu.A = 0;
	cpu.B = 0;
	cpu.eval();

	std::cout << (int)cpu.B << std::endl;
	
	return 0;
}