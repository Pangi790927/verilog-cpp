#ifndef ASM_INSTR
#define ASM_INSTR

#include "regs_map.h"
#include "str_helper.h"
#include "debug.h"

struct Instr {
	int line_cnt;

	Instr(int line_cnt) : line_cnt(line_cnt) {}
	virtual void polymorfic() {}
};

struct AsmInstr : Instr {
	std::string instr;
	std::string args;
	int wordCount;
	int free_reg = -1;

	AsmInstr(int line_cnt, std::string instr, std::string args)
	: Instr(line_cnt), instr(instr), args(args), wordCount(1) {}
};

struct LabelInstr : Instr {
	std::string label;
	bool isLocal = false;
	std::string parrent;

	LabelInstr(int line_cnt, std::string label, std::string parrent = "")
	: label(label), parrent(parrent), Instr(line_cnt)
	{
		if (parrent != "")
			isLocal = true;
	}
};

#endif