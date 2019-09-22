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
	int word_cnt;
	uint32_t addr;
};

#endif
