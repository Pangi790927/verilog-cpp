#ifndef LIB_DEBUG_H
#define LIB_DEBUG_H

#include <string>
#include <iostream>
#include <fstream>

#include "json.h"
#include "asm_instr.h"
#include "exceptions.h"

#ifndef REGS_PATH
#define REGS_PATH "regs.json"
#endif

#ifndef INSTR_PATH
#define INSTR_PATH "instr.json"
#endif

void load();

void unload();

std::string disas(uint32_t instr_code, uint32_t peek_arg);

#endif