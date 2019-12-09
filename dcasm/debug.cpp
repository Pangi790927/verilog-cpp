#include "debug.h"

nlohmann::json j_instr;
nlohmann::json j_regs;

void load() {
	std::ifstream in_regs(REGS_PATH);
	std::ifstream in_instr(INSTR_PATH);

	if (!in_regs.good())
		throw EXCEPTION("could not open input file regs");
	if (!in_instr.good())
		throw EXCEPTION("could not open input file instr");

	in_regs >> j_regs;
	in_instr >> j_instr;
}

void unload() {
	j_instr = nullptr;
	j_regs = nullptr;
}

std::string disas(uint32_t instr_code, uint32_t peek_arg) {
	using namespace nlohmann;

	if (j_instr == nullptr || j_instr == nullptr) {
		throw EXCEPTION("Call load() prior to this function to load instruction tables");
	}

	AsmBin instr;
	instr.code = instr_code;

	int op_cnt = 0;
	std::string op;
	std::string comp;
	std::string reg;
	std::string reg1;
	std::string reg2;

	for (auto&& [ins, ins_code] : j_instr.get<json::object_t>()) {
		if (own_stol(ins_code["code"]) == instr.op) {
			op = ins;
			op_cnt = own_stol(ins_code["op_cnt"]);
		}
	}

	for (auto&& [reg_str, reg_code] : j_regs.get<json::object_t>()) {
		if (instr.reg == own_stol(reg_code))
			reg = reg_str;
		if (instr.reg1 == own_stol(reg_code))
			reg1 = reg_str;
		if (instr.reg2 == own_stol(reg_code))
			reg2 = reg_str;
	}

	switch (instr.mod) {
		case 0:
			comp = sformat("%s", reg1);
			break;
		case 1:
			comp = sformat("%s", std::to_string(peek_arg));
			break;
		case 2:
			comp = sformat("[%s]", std::to_string(peek_arg));
			break;
		case 3:
			comp = sformat("[%s]", reg1);
			break;
		case 4:
			comp = sformat("[%s + %s]", reg1, reg2);
			break;
		case 5:
			comp = sformat("[%s + %s]", reg1, std::to_string(peek_arg));
			break;
		case 6:
			comp = sformat("[%s + %s + %s]",
					reg1, reg2, std::to_string(peek_arg));
			break;
	}

	std::string arg1;
	std::string arg2;
	if (op_cnt == 1) {
		arg1 = " " + comp;
	}
	if (op_cnt == 2) {
		arg1 = " " + (instr.dir ? reg : comp) + ", ";
		arg2 = (!instr.dir ? reg : comp);
	}

	if (op == "")
		return "";

	return sformat("[op: %3d mod: %1d reg: %2d reg1: %2d reg2: %2d dir: %1d] %s%s%s",
			instr.op, instr.mod, instr.reg, instr.reg1, instr.reg2, instr.dir,
			op, arg1, arg2);
}
