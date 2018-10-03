#include <iostream>
#include "obj_dir/Vcpu.h"
#include "verilated.h"

int main(int argc, char const *argv[])
{
	Vcpu cpu;
	cpu.A = 0;
	cpu.eval();

	std::cout << cpu.B << std::endl;
	
	return 0;
}