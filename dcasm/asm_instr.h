#ifndef ASM_INSTR
#define ASM_INSTR

struct Instr
{
	// int lineNumber;

	// Instr() : lineNumber(-1) {}
	// Instr(int lineNumber) : lineNumber(lineNumber) {}
};

struct AsmInstr : Instr {
	std::string instr;
	std::string args;
	int wordSize;

	AsmInstr(std::string instr, std::string args) : instr(instr), args(args), wordSize(1) {}
};

struct LabelInstr : Instr {
	std::string label;
	bool isLocal = false;
	std::string parrent;

	LabelInstr(std::string label, std::string parrent = "")
	: label(label), parrent(parrent) {}
};

#endif