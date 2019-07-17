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

	int decode_mode() {
		auto arg_vec = ssplit(args, ",");
		if (arg_vec.size() > 2) {
			throw std::runtime_error("can't have more than 2 operands");
		}

		if (arg_vec.size() == 0) {
			return -1; // todo
		}

		if (arg_vec.size() == 1) {
			return -1; // todo
		}

		return decode_two_args();
	}

	int decode_two_args() {
		std::cout << "2 arg instr" << std::endl;

		auto arg_vec = ssplit(args, ",");
		int dir = decode_direction();

		std::string f_arg = (dir == 1) ? trim(arg_vec[0]) : trim(arg_vec[1]);
		std::string s_arg = (dir == 1) ? trim(arg_vec[1]) : trim(arg_vec[0]);

		/* Check Register mode */
		if (regs_map.find(f_arg) != regs_map.end() && 
			regs_map.find(s_arg) != regs_map.end()) {

			std::cout << "Register mode " << f_arg << ", " << s_arg << std::endl;
			return 0;
		}

		/* Check Immediate mode */
		try {
			unsigned long val = stoll(s_arg, nullptr, 0);

			if (regs_map.find(f_arg) != regs_map.end()) {

				std::cout << "Immediate mode " << f_arg << " <= " << val << std::endl;
				return 1;
			}
		} catch (std::invalid_argument& ignored) {

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