#ifndef ASM_INSTR
#define ASM_INSTR

#include "str_helper.h"
#include "debug.h"

// label:
	// .local
// label.local
// (line, dir, label, line, word_cnt, addr)
struct AsmInstr {
	bool is_local;
	bool is_label;
	bool is_instr;
	bool is_instr0;
	bool is_instr1;
	bool is_instr2;
	int dir;		// 0 or 1
	std::string parrent_label;
	std::string line;
	int line_nr;
	int word_cnt;
	uint32_t addr;
};

struct AsmBin {
	union {
		struct __attribute__((__packed__)) {
			uint32_t op 	: 8;
			uint32_t dir	: 1;
			uint32_t mod	: 3;
			uint32_t reg	: 5;
			uint32_t reg1	: 5;
			uint32_t reg2	: 5;
		};
		uint32_t code = 0;
	};

	std::string to_string() {
		char buff[128]{0};
		snprintf(buff, sizeof(buff),
				"%#x -> op[%#x]dir[%d]mod[%d]reg[%d]reg1[%d]reg2[%d]",
				code, op, dir, mod, reg, reg1, reg2);
		return buff;
	}
};



#endif
