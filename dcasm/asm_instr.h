#ifndef ASM_INSTR
#define ASM_INSTR

struct Instr {
	virtual void polymorfic() {}
};

struct AsmInstr : Instr {
	std::string instr;
	std::string args;
	int wordCount;

	AsmInstr(std::string instr, std::string args) : instr(instr), args(args), wordCount(1) {}
};

struct LabelInstr : Instr {
	std::string label;
	bool isLocal = false;
	std::string parrent;

	LabelInstr(std::string label, std::string parrent = "")
	: label(label), parrent(parrent) {}
};

#endif