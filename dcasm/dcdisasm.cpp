#include <iostream>
#include <fstream>

#include "json.h"
#include "asm_instr.h"
#include "exceptions.h"
#include "str_helper.h"
#include "debug.h"

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

int main(int argc, char const *argv[]) try {
	if (argc < 3) {
		std::cerr << "Syntax: " << argv[0] <<
				" <input_file.o> <output_file.asm>" << std::endl;
		return 1;
	}

	load();

	std::ofstream out(argv[2]);
	if (!out.good())
		throw EXCEPTION("could not open output file");

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
		auto res = disas(in_buff[i], (i + 1) != in_size ? in_buff[i + 1] : 0); 
		if (res != "")
			out << res << std::endl;
		if (instr.mod == 1 || instr.mod == 2 || instr.mod == 5 || instr.mod == 6)
			i++;
	}

	unload();

	return 0;
}
catch (std::exception& err) {
	std::cout << "Exception while disassembling: " << err.what() << std::endl;
}
