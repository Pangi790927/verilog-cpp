#ifndef ASM_INSTR
#define ASM_INSTR

#include "regs_map.h"
#include "str_helper.h"

struct Instr {
	virtual void polymorfic() {}
};

struct AsmInstr : Instr {
	std::string instr;
	std::string args;
	int wordCount;

	AsmInstr(std::string instr, std::string args) : instr(instr), args(args), wordCount(1) {}

	int decode_direction() {
		auto arg_vec = ssplit(args, ",");
		if (arg_vec.size() > 2)
			throw std::runtime_error("can't have more than 2 operands");

	}
};

struct LabelInstr : Instr {
	std::string label;
	bool isLocal = false;
	std::string parrent;

	LabelInstr(std::string label, std::string parrent = "")
	: label(label), parrent(parrent) {}
};

#endif