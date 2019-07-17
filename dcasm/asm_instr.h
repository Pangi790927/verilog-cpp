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

	int decode_direction() {
		auto arg_vec = ssplit(args, ",");
		if (arg_vec.size() > 2) {
			throw EXCEPTION("can't have more than 2 operands, [at line: %d]", line_cnt);
		}
		if (arg_vec.size() <= 1) {
			if (arg_vec.size() == 1) {
				if (regs_map.find(trim(arg_vec[1])) != regs_map.end()) {
					free_reg = regs_map[trim(arg_vec[0])];
				}
			}
			return 0;
		}
		if (regs_map.find(trim(arg_vec[1])) != regs_map.end()) {
			free_reg = regs_map[trim(arg_vec[1])];
			return 0;
		}
		else if (regs_map.find(trim(arg_vec[0])) != regs_map.end()) {
			free_reg = regs_map[trim(arg_vec[0])];
			return 1;
		}
		else {
			throw EXCEPTION("One of the operands must be a register, [at line: %d]",
					line_cnt);
		}
	}
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