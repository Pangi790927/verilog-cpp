#ifndef PARSER_H
#define PARSER_H

#include <bitset>
#include <vector>
#include <regex>
#include <set>

#include "str_helper.h"
#include "asm_instr.h"
#include "debug.h"
#include "json.h"

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
	std::string last_label;
	std::ifstream in;
	std::ofstream out;
	nlohmann::json j_instr;
	nlohmann::json j_regs;
	nlohmann::json j_match;
	std::regex label_re;
	std::regex local_re;
	std::regex instr0_re;
	std::regex instr1_re;
	std::regex instr2or_re;
	std::regex instr2ro_re;

	std::map<std::string, AsmInstr *> label_map;
	std::vector<std::shared_ptr<AsmInstr>> asm_instr;

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

		label_re = std::regex(GET_STR(j_match, "label"));
		local_re = std::regex(GET_STR(j_match, "local_label"));
		instr0_re = std::regex(GET_STR(j_match, "instr"));
		instr1_re = std::regex(GET_STR(j_match, "instr_op"));
		instr2or_re = std::regex(GET_STR(j_match, "instr_op_re"));
		instr2ro_re = std::regex(GET_STR(j_match, "instr_re_op"));
	}

	void parse() {
		std::string line;
		int line_cnt = 1;

		std::cout << "============= PARSING BEGIN =============" << std::endl;
		while (getline(in, line)) {
			parseLine(line, line_cnt++);
		}
		///
		std::cout << "============== PARSING END ==============" << std::endl;
	}

	void translate() {
		std::cout << "=========== TRANSLATING BEGIN ===========" << std::endl;
		std::vector<uint32_t> instructions;
		std::smatch match;

		for (auto& instr : asm_instr) {
			AsmBin instruction;

			if (instr->is_local) {
				std::cout << "Label" << std::endl;
				continue;
			}

			if (instr->is_label)
			{
				std::cout << "Local label" << std::endl;
				continue;
			}

			if (instr->is_instr)
			{
				std::cout << "Instruction" << instr->line << std::endl;
				std::string instr_alias = match.str(0);
				instr_alias = trim(instr_alias);

				std::cout << "Instr: " << instr_alias << std::endl;

				std::string instr_code = GET_STR(j_instr[instr_alias], "code");
				int code = std::stoi(instr_code, nullptr, 0);

				instruction.op = code;			// append instruction code
				instruction.dir = instr->dir;	// append direction
				std::string rest = match.suffix().str();

				if (instr->is_instr0) {

				}
				if (instr->is_instr0) {

				}

				std::cout << "Code for instr: " << instr_code
						<< " => " << code << std::endl;					
				std::cout << instruction.to_string() << std::endl;
			}
		}
		std::cout << "============ TRANSLATING END ============" << std::endl;
	}

	void parseLine(std::string &line, int line_cnt) {
		static std::regex comment_regex(GET_STR(j_match, "comment"));
		static std::regex constant_regex(
				GET_STR(j_match["macro"], "__CONSTANT__"));
		static std::regex local_regex(
				GET_STR(j_match["macro"], "__LOCAL_LB__"));
		static std::regex label_regex(
				GET_STR(j_match["macro"], "__LABEL__"));
		line = std::regex_replace(line, comment_regex, "");

		bool is_label = std::regex_match(line, label_re);
		bool is_local = std::regex_match(line, local_re);
		bool is_instr0 = std::regex_match(line, instr0_re);
		bool is_instr1 = std::regex_match(line, instr1_re);
		bool is_op_re = std::regex_match(line, instr2or_re);
		bool is_re_op = std::regex_match(line, instr2ro_re);

		if (!(is_label || is_local || is_instr0 || is_instr1 || is_op_re ||
				is_re_op))
		{
			if (ltrim(line) == "")
				return ;
			else
				throw EXCEPTION("syntax error [line %d]: %s",
						line_cnt, line.c_str()); 
		}

		static uint32_t current_addr = 0;
		static std::string curr_label = "__main_file_label__";
		bool has_constant = std::regex_search(line, constant_regex);

		std::smatch sm_label;
		std::smatch sm_local;
		std::regex_search(line, sm_label, label_regex);
		std::regex_search(line, sm_local, local_regex);

		if (is_label)
			curr_label = sm_label.str();

		AsmInstr instr = {
			.is_label = is_label,
			.is_local = is_local,
			.is_instr0 = is_instr0,
			.is_instr1 = is_instr1,
			.is_instr2 = is_op_re || is_re_op,
			.is_instr = is_instr0 || is_instr1 || is_re_op || is_op_re,
			.dir = is_re_op, // all other are direction 0
			.parrent_label = curr_label,
			.line = line,
			.word_cnt = !(is_local || is_label) ? 1 + has_constant : 0,
			.addr = current_addr
		};
		current_addr += instr.word_cnt * 4;
		asm_instr.emplace_back(new AsmInstr(instr));
		if (is_label) {
			label_map[sm_label.str()] = asm_instr.back().get();
		}
		if (is_local) {
			label_map[curr_label + sm_local.str()] = asm_instr.back().get();
		}
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
					}
				}
			}
		}
	}
};

#endif