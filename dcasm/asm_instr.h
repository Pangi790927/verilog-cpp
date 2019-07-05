#ifndef ASM_INSTR
#define ASM_INSTR

struct AsmInstr {
	std::string instr;
	std::string args;

	AsmInstr() {};
	AsmInstr(std::string instr, std::string args) : instr(instr), args(args) {};
};

std::vector<AsmInstr> instructions;

#endif