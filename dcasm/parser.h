#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <regex>
#include <set>

#include "str_helper.h"
#include "instr_map.h"
#include "asm_instr.h"
#include "debug.h"
#include "json.h"

// instr		{space}__CMD__{space}
// instr_op		{space}__CMD__{space}__DIRECTION__{space}
// instr_re_op 	{space}__CMD__{space}__REGS__{,}__DIRECTION__{space}
// instr_op_re 	{space}__CMD__{space}__DIRECTION__{,}__REGS__{space}
// direction_re	{...}


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

struct Parser {
	std::vector<Instr *> asmInstr;
	std::string last_label;
	std::ifstream in;
	std::ofstream out;
	nlohmann::json j_instr;
	nlohmann::json j_regs;
	nlohmann::json j_match;

	Parser (std::string fileIn, std::string fileOut)
	: in(fileIn.c_str()), out(fileOut.c_str())
	{
		std::ifstream in_regs("regs.json");
		std::ifstream in_instr("instr.json");
		std::ifstream in_match("matchers.json");
		if (!in.good())
			throw EXCEPTION("could not open input file");
		if (!out.good())
			throw EXCEPTION("could not open output file");
		if (!in_regs.good())
			throw EXCEPTION("could not open input file regs");
		if (!in_instr.good())
			throw EXCEPTION("could not open input file instr");
		if (!in_match.good())
			throw EXCEPTION("could not open input file matchers");
		in_regs >> j_regs;
		in_instr >> j_instr;
		in_match >> j_match;

		expand_macros();
		using namespace nlohmann;
		for (auto&& [key, val] : j_match["macro"].get<json::object_t>()) {
			printf("[%s] = %s\n", key.c_str(), val.get<std::string>().c_str());
		}
		for (auto&& [key, val] : j_match.get<json::object_t>()) {
			if (key != "macro")
				printf("[%s] = %s\n", key.c_str(),
						val.get<std::string>().c_str());
		}
	}

	void parse() {
		std::string line;
		int line_cnt = 1;

		std::cout << "============= PARSING BEGIN =============" << std::endl;
		while (getline(in, line)) {
			parseLine(line, line_cnt++);
		}
		std::cout << "============== PARSING END ==============" << std::endl;
	}

	void parseLine(std::string &line, int line_cnt) {
		std::regex comment_regex(GET_STR(j_match, "comment"));
		line = std::regex_replace(line, comment_regex, "");

		static std::regex label_re(GET_STR(j_match, "label"));
		static std::regex local_re(GET_STR(j_match, "local_label"));
		static std::regex instr0_re(GET_STR(j_match, "instr"));
		static std::regex instr1_re(GET_STR(j_match, "instr_op"));
		static std::regex instr2or_re(GET_STR(j_match, "instr_op_re"));
		static std::regex instr2ro_re(GET_STR(j_match, "instr_re_op"));

		if (std::regex_match(line, label_re)) {
			printf("label:       %s\n", ltrim(line).c_str());
		}

		if (std::regex_match(line, local_re)) {
			printf("local label: %s\n", ltrim(line).c_str());
		}

		if (std::regex_match(line, instr0_re)) {
			printf("instr 0op:   %s\n", ltrim(line).c_str());
		}

		if (std::regex_match(line, instr1_re)) {
			printf("instr 1op:   %s\n", ltrim(line).c_str());
		}

		if (std::regex_match(line, instr2ro_re) ||
				std::regex_match(line, instr2or_re))
		{
			printf("instr 2op:   %s\n", ltrim(line).c_str());
		}

		asmInstr.emplace_back(line, dir, label, line, word_cnt, addr)
		// asmInstr.push_back(new AsmInstr(line_cnt, command, args));
		return ;
	}

	void expand_regs() {
		using namespace nlohmann;
		std::string reg_regex = "(";
		for (auto&& [reg, val] : j_regs.get<json::object_t>()) {
			reg_regex += reg + "|";
		}
		j_match["macro"]["__REGS__"] =
				reg_regex.substr(0, reg_regex.size() - 1) + ")";
	}

	void expand_instrs() {
		using namespace nlohmann;
		std::string reg_instr = "(";
		for (auto&& [instr, val] : j_instr.get<json::object_t>()) {
			reg_instr += instr + "|";
		}
		j_match["macro"]["__INSTRS__"] =
				reg_instr.substr(0, reg_instr.size() - 1) + ")";
	}

	bool needs_expansion(const std::string& reg_expr) {
		using namespace nlohmann;
		for (auto&& [key, macro] : j_match["macro"].get<json::object_t>()) {
			if (macro.get<std::string>().find(reg_expr) != std::string::npos) {
				return true;
			}
		}
		return false;
	}

	void expand_macro(const std::string& macro, std::set<std::string> in_exp) {
		using namespace nlohmann;
		for (auto&& [key, val] : j_match["macro"].get<json::object_t>()) {
			if (j_match["macro"][macro].get<std::string>().find(key) !=
					std::string::npos)
			{
				if (needs_expansion(key)) {
					if (in_exp.find(key) != in_exp.end()) {
						throw std::runtime_error("recursive macro expansion");
					}
					else {
						in_exp.insert(key);
						expand_macro(key, in_exp);
						in_exp.erase(key);
					}
				}
				std::string new_macro = j_match["macro"][macro];
				replaceAll(new_macro, key, val.get<std::string>());
				j_match["macro"][macro] = new_macro;
			}
		}
	}

	void expand_macros() {
		using namespace nlohmann;
		expand_regs();
		expand_instrs();
		for (auto&& [key, val] : j_match["macro"].get<json::object_t>()) {
			if (needs_expansion(val.get<std::string>())) {
				std::set<std::string> in_expansion;
				in_expansion.insert(key);
				expand_macro(key, in_expansion);
			}
		}
		for (auto&& [key, _] : j_match.get<json::object_t>()) {
			if (key != "macro") {
				for (auto&& [macro_key, macro] : j_match["macro"].
						get<json::object_t>())
				{
					auto val = j_match[key];
					if (val.get<std::string>().find(macro_key) !=
							std::string::npos)
					{
						// printf("%s\n", val.get<std::string>().c_str());
						std::string new_str = val.get<std::string>();
						// printf("new: %s\n", new_str.c_str());
						replaceAll(new_str, macro_key, macro.get<std::string>());
						// printf("new: %s\n", new_str.c_str());
						j_match[key] = new_str;
						printf("key: %s\n", key.c_str());
						printf("replaced: %s\n", macro_key.c_str());
						printf("with: %s\n", macro.get<std::string>().c_str());
						printf("new_str: %s\n", new_str.c_str());
					}
				}
			}
		}
	}
};

#endif