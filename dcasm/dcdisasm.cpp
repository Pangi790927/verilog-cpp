#include <iostream>
#include <fstream>

#include "json.h"
#include "asm_instr.h"
#include "debug.h"
#include "str_helper.h"

#define GET_STR(jobj, str)\
([&]() -> std::string {\
	if (jobj.find(str) != jobj.end()) {\
		return jobj[str].get<std::string>();\
	}\
	return "";\
}())\

#define GET_INT(jobj, str)\
([&]() -> int {\
	if (jobj.find(str) != jobj.end()) {\
		return std::stoi(jobj[str].get<std::string>());\
	}\
}())\

std::vector<uint8_t> read_file(std::string path) {
	std::ifstream file(path.c_str(), std::ios::binary);
	if (!file.good()) {
		throw EXCEPTION("No such file or directory %s", path.c_str());
	}
	std::vector<uint8_t> buff{
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	};
	return buff;
}

std::string disas(uint32_t instr_code, uint32_t peek_arg,
		const nlohmann::json& j_regs, const nlohmann::json& j_instr)
{
	using namespace nlohmann;

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

int main(int argc, char const *argv[]) try {
	if (argc < 3) {
		std::cerr << "Syntax: " << argv[0] <<
				" <input_file.o> <output_file.asm>" << std::endl;
		return 1;
	}

	std::ofstream out(argv[2]);
	std::ifstream in_regs("regs.json");
	std::ifstream in_instr("instr.json");

	if (!out.good())
		throw EXCEPTION("could not open output file");
	if (!in_regs.good())
		throw EXCEPTION("could not open input file regs");
	if (!in_instr.good())
		throw EXCEPTION("could not open input file instr");

	nlohmann::json j_instr;
	nlohmann::json j_regs;

	in_regs >> j_regs;
	in_instr >> j_instr;

	std::vector<uint8_t> buff = read_file(argv[1]);
	if (buff.size() == 0) {
		std::cout << "in file is empty, will exit" << std::endl;
		return 0;
	}
	size_t in_size = buff.size() / 4;
	uint32_t *in_buff = (uint32_t *)&buff[0];

	for (size_t i = 0; i < in_size; i++) {
		AsmBin instr;
		instr.code = in_buff[i];
		auto res = disas(in_buff[i], (i + 1) != in_size ? in_buff[i + 1] : 0,
				j_regs, j_instr); 
		if (res != "")
			out << res << std::endl;
		if (instr.mod == 1 || instr.mod == 2 || instr.mod == 5 || instr.mod == 6)
			i++;
	}

	return 0;
}
catch (std::exception& err) {
	std::cout << "Exception while disassembling: " << err.what() << std::endl;
}
