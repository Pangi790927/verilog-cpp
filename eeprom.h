#include <string>
#include <iostream>
#include "ram.h"
#include "debug.h"

struct EEPROM {

	EEPROM(RAM &ram, std::string bios_path) {
		std::vector<uint8_t> instructions = read_file(bios_path);

		std::copy(instructions.begin(), instructions.end(), ram.mem.get());
		std::cout << "Successfully loaded " << bios_path << " to ram" << std::endl;
	}

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
};