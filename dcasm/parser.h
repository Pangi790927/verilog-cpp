#ifndef PARSER_H
#define PARSER_H

#include <bitset>
#include <vector>
#include <regex>
#include <set>

#include "str_helper.h"
#include "asm_instr.h"
#include "exceptions.h"
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
	: in(fileIn.c_str()), out(fileOut.c_str(), std::ios::binary)
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
			if (val.is_string())
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
		std::cout << "============== PARSING END ==============" << std::endl;
	}

	void translate() {
		std::cout << "=========== TRANSLATING BEGIN ===========" << std::endl;
		std::vector<uint32_t> memory;
		std::vector<uint32_t> data_memory;
		std::smatch match;

		auto append_static_data = [&data_memory, &memory] {
			int i = 0;
			uint8_t bytes[4];
			for (auto byte : data_memory) {
				bytes[i] = byte;
				i++;
				if (i >= 4) {
					i = 0;
					memory.push_back(*(uint32_t *)bytes);
				}
			}
			if (i != 0)
				memory.push_back(*(uint32_t *)bytes);
			data_memory.clear();
		};

		for (auto& instr : asm_instr) {
			AsmBin instruction;

			/* those use no memory */
			if (instr->is_local) {
				continue;
			}

			if (instr->is_label) {
				continue;
			}

			/* those will use memory */
			if (instr->is_data) {
				auto vec = parse_data(instr.get());
				data_memory.insert(data_memory.end(), vec.begin(), vec.end());
				printf("line \"%s\" will be transformed in:\n\t -> ",
						instr->line.c_str());
				for (auto&& byte : vec)
					printf("%#x, ", byte);
				printf("\n\n");
				continue;
			}
			else {
				append_static_data();	
			}

			if (instr->is_instr)
			{
				static std::regex instr_regex(
						GET_STR(j_match["macro"], "__INSTRS__"));
				static std::regex instr_sp_regex(
						GET_STR(j_match["macro"], "__INSTR_SP__"));
				static std::regex reg_regex(
						GET_STR(j_match["macro"], "__REGS__"));
				static std::regex comp_regex(
						GET_STR(j_match["macro"], "__COMPOSED__"));

				std::string instr_alias;
				if (!instr->is_instr0) {
					std::regex_search(instr->line, match, instr_sp_regex);
					instr_alias = match.str(0);
					instr_alias = trim(instr_alias);
				}
				else {
					std::regex_search(instr->line, match, instr_regex);
					instr_alias = match.str(0);
					instr_alias = trim(instr_alias);	
				}

				std::string instr_code = GET_STR(j_instr[instr_alias], "code");
				int code = std::stoi(instr_code, nullptr, 0);

				instruction.op = code;			// append instruction code
				instruction.dir = instr->dir;	// append direction
				
				std::string rest = match.suffix();
				uint32_t const_val = 0;
				bool has_const_val = false;

				if (instr->is_instr0) {
					; // nothing more to do
				}
				else if (instr->is_instr1) {
					std::regex_search(rest, match, comp_regex);
					std::string comp = match.str(0);

					instruction.mod = find_mode(comp, instr.get());

					if (has_const(comp, instruction.mod, instr.get())) {
						has_const_val = true;
						const_val = find_const(comp, instruction.mod,
								instr->parrent_label, instr.get());
					}

					instruction.reg1 = find_reg1(comp, instruction.mod, instr.get());
					instruction.reg2 = find_reg2(comp, instruction.mod, instr.get());
				}
				else if (instr->is_instr2 && instr->dir == 0) {
					std::regex_search(rest, match, comp_regex);
					std::string comp = match.str();
					std::string rest2 = match.suffix().str();

					std::regex_search(rest2, match, reg_regex);
					instruction.reg = find_reg(match.str(), instr.get());
					instruction.mod = find_mode(comp, instr.get());

					if (has_const(comp, instruction.mod, instr.get())) {
						has_const_val = true;
						const_val = find_const(comp, instruction.mod,
								instr->parrent_label, instr.get());
					}

					instruction.reg1 = find_reg1(comp, instruction.mod, instr.get());
					instruction.reg2 = find_reg2(comp, instruction.mod, instr.get());
				}
				else if (instr->is_instr2 && instr->dir == 1) {
					std::regex_search(rest, match, reg_regex);
					instruction.reg = find_reg(match.str(), instr.get());
					std::string rest2 = match.suffix().str();

					std::regex_search(rest2, match, comp_regex);
					std::string comp = match.str();
					instruction.mod = find_mode(comp, instr.get());

					if (has_const(comp, instruction.mod, instr.get())) {
						has_const_val = true;
						const_val = find_const(comp, instruction.mod,
								instr->parrent_label, instr.get());
					}

					instruction.reg1 = find_reg1(comp, instruction.mod, instr.get());
					instruction.reg2 = find_reg2(comp, instruction.mod, instr.get());
				}
				std::cout << "Instr line: " << instr->line << std::endl;
				std::cout << instruction.to_string() << std::endl;
				memory.push_back(instruction.code);
				if (has_const_val) {
					memory.push_back(const_val);
					std::cout << "const val: " << const_val << std::endl;
				}
			}
			printf("\n");
		}
		std::cout << "============ TRANSLATING END ============" << std::endl;

		append_static_data();
		if (memory.size() != 0)
			out.write((char *)&memory[0], memory.size() * sizeof(memory[0]));
		else
			throw EXCEPTION("No resulting data");
	}

	void parseLine(std::string &line, int line_cnt) {
		static std::regex comment_regex(GET_STR(j_match, "comment"));
		static std::regex data_regex(GET_STR(j_match, "data"));
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
		bool is_data = std::regex_match(line, data_regex);

		if (!(is_label || is_local || is_instr0 || is_instr1 || is_op_re ||
				is_re_op || is_data))
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
			.is_data = is_data,
			.is_instr0 = is_instr0,
			.is_instr1 = is_instr1,
			.is_instr2 = is_op_re || is_re_op,
			.is_instr = is_instr0 || is_instr1 || is_re_op || is_op_re,
			.dir = is_re_op, // all other are direction 0
			.parrent_label = curr_label,
			.line = line,
			.line_nr = line_cnt,
			.addr = current_addr
		};
		/* because data can have unaligned labels we need to treat it
		differently */
		if (instr.is_data) {
			instr.word_cnt = 0;
			current_addr += compute_data_space(&instr);
		}
		else {
			instr.word_cnt = !(is_local || is_label) ? 1 + has_constant : 0;
			current_addr = (current_addr / 4 + !!(current_addr % 4)) * 4;
			current_addr += instr.word_cnt * 4;
		}
		asm_instr.emplace_back(new AsmInstr(instr));
		if (is_label) {
			label_map[sm_label.str()] = asm_instr.back().get();
		}
		if (is_local) {
			label_map[curr_label + sm_local.str()] = asm_instr.back().get();
		}
	}

	int compute_data_space(AsmInstr *instr) {
		return parse_data(instr).size();
	}

	std::vector<uint8_t> parse_data_num(std::string arg, int data_align) {
		std::vector<uint8_t> bytes;
		uint8_t num[4];
		switch (data_align) {
			case 1:
				*(uint8_t *)num = stoi(arg);
				break;
			case 2:
				*(uint8_t *)num = stoi(arg);
				break;
			case 4:
				*(uint8_t *)num = stoi(arg);
				break;
			default:
				EXCEPTION("unknown padding %d", data_align);
		}
		for (int i = 0; i < data_align; i++)
			bytes.push_back(num[i]);
		return bytes;
	}

	std::vector<uint8_t> parse_data_chr(std::string arg, int data_align) {
		if (arg.size() > 1)
			return parse_data_num(std::to_string((int)arg[1]), data_align);
		else
			throw EXCEPTION("char constant must have the following format: '.',"
					"whre . can be any character");
	}

	std::vector<uint8_t> parse_data_str(std::string arg, int data_align) {
		std::vector<uint8_t> bytes;
		for (auto chr : arg)
			bytes.push_back(chr);
		while (bytes.size() % data_align != 0)
			bytes.push_back(0);
		return bytes;
	}

	std::vector<uint8_t> parse_data(AsmInstr *instr) {
		static std::regex str_regex(GET_STR(j_match["macro"], "__STRING__"));
		static std::regex chr_regex(GET_STR(j_match["macro"], "__CHAR__"));
		static std::regex num_regex(GET_STR(j_match["macro"], "__NUMBER__"));
		static std::regex space_regex(GET_STR(j_match["macro"], "__SPACE__"));
		static std::regex data_regex(GET_STR(j_match["macro"], "__DATA__"));
		std::vector<uint8_t> bytes;

		int data_align = 0;
		std::smatch match;
		if (!std::regex_search(instr->line, match, data_regex)) {
			throw EXCEPTION("not data [line %d]: %s",
						instr->line_nr, instr->line.c_str());
		}
		else {
			for (auto&& elem : j_match["data_aling"]) {
				if (trim(match.str()) == GET_STR(elem, "name"))
					data_align = elem["padd"].get<int>();
			}
		}
		std::string args = ltrim(match.suffix().str());
		while (args != "") {
			if (args.size() > 0 && args[0] == ',')
				args = args.substr(1, std::string::npos);
			if (std::regex_search(args, match, num_regex) && 
					ltrim(match.prefix().str()) == "")
			{
				auto vec = parse_data_num(match.str(), data_align);
				bytes.insert(bytes.end(), vec.begin(), vec.end());
			}
			else if (std::regex_search(args, match, chr_regex) && 
					ltrim(match.prefix().str()) == "")
			{
				auto vec = parse_data_chr(match.str(), data_align);
				bytes.insert(bytes.end(), vec.begin(), vec.end());
			}
			else if (std::regex_search(args, match, str_regex) && 
					ltrim(match.prefix().str()) == "")
			{
				auto vec = parse_data_str(match.str(), data_align);
				bytes.insert(bytes.end(), vec.begin(), vec.end());
			}
			args = match.suffix().str();
		}

		return bytes;
	}

	uint32_t find_mode(std::string composed, AsmInstr *instr) {
		std::smatch match;

		static std::regex mod0_regex(
						GET_STR(j_match["macro"], "__MOD0__"));

		if (std::regex_match(composed, match, mod0_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD0__ instr -> "<< composed << std::endl;
			return 0;
		}

		static std::regex mod1_regex(
						GET_STR(j_match["macro"], "__MOD1__"));

		if (std::regex_match(composed, match, mod1_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD1__ instr -> "<< composed << std::endl;
			return 1;
		}

		static std::regex mod2_regex(
						GET_STR(j_match["macro"], "__MOD2__"));

		if (std::regex_match(composed, match, mod2_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD2__ instr -> "<< composed << std::endl;
			return 2;
		}

		static std::regex mod3_regex(
						GET_STR(j_match["macro"], "__MOD3__"));

		if (std::regex_match(composed, match, mod3_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD3__ instr -> "<< composed << std::endl;
			return 3;
		}

		static std::regex mod4_regex(
						GET_STR(j_match["macro"], "__MOD4__"));

		if (std::regex_match(composed, match, mod4_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD4__ instr -> "<< composed << std::endl;
			return 4;
		}

		static std::regex mod5_regex(
						GET_STR(j_match["macro"], "__MOD5__"));

		if (std::regex_match(composed, match, mod5_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD5__ instr -> "<< composed << std::endl;
			return 5;
		}

		static std::regex mod6_regex(
						GET_STR(j_match["macro"], "__MOD6__"));

		if (std::regex_match(composed, match, mod6_regex)) {
			std::string instr_reg = match.str(0);
			std::cout << "__MOD6__ instr -> "<< composed << std::endl;
			return 6;
		}

		throw new EXCEPTION("Unkown instruction mode: : %s [line %d] %s",
						match.str().c_str(), instr->line_nr, instr->line.c_str());
	}

	uint32_t find_reg(std::string reg_part, AsmInstr *instr) {
		std::smatch match;

		static std::regex reg_regex(
						GET_STR(j_match["macro"], "__REGS__"));

		if (!std::regex_match(reg_part, match, reg_regex)) {
			throw new EXCEPTION("Register not found in the reg_part: %s [line %d] %s",
						match.str().c_str(), instr->line_nr, instr->line.c_str());
		}

		return own_stol(GET_STR(j_regs, reg_part));
	}

	uint32_t find_reg1(std::string comp, uint32_t mode, AsmInstr *instr) {
		static std::regex reg_regex(GET_STR(j_match["macro"], "__REGS__"));

		std::smatch match;
		if (mode == 1)
			return 0;
		std::regex_search(comp, match, reg_regex);
		return own_stol(GET_STR(j_regs, match.str()));
	}

	uint32_t find_reg2(std::string comp, uint32_t mode, AsmInstr *instr) {
		static std::regex reg_regex(GET_STR(j_match["macro"], "__REGS__"));

		std::smatch match;
		if (mode == 0 || mode == 1 || mode == 2 || mode == 3 || mode == 5)
			return 0;

		std::regex_search(comp, match, reg_regex);
		comp = match.suffix().str();

		std::regex_search(comp, match, reg_regex);
		return own_stol(GET_STR(j_regs, match.str()));
	}

	bool has_const(std::string comp, uint32_t mode, AsmInstr *instr) {
		switch(mode) {
			case 0: return false;
			case 1: return true;
			case 2: return true;
			case 3: return false;
			case 4: return false;
			case 5: return true;
			case 6: return true;
			default: throw EXCEPTION("mode not known for instr[line %d] %s",
				instr->line_nr, instr->line.c_str());
		}
		return false;
	}

	uint32_t find_const(std::string comp, uint32_t mode,
			std::string parrent_label, AsmInstr *instr)
	{
		static std::regex label_regex(GET_STR(j_match["macro"], "__LABEL__"));		
		static std::regex local_regex(GET_STR(j_match["macro"], "__LOCAL_LB__"));		
		static std::regex number_regex(GET_STR(j_match["macro"], "__NUMBER__"));		
		static std::regex char_regex(GET_STR(j_match["macro"], "__CHAR__"));		
		static std::regex reg_regex(GET_STR(j_match["macro"], "__REGS__"));
		static std::regex const_regex(GET_STR(j_match["macro"], "__CONSTANT__"));		

		std::smatch match;

		if (mode == 0 || mode == 3 || mode == 4)
			return 0;
		if (mode == 5 || mode == 6) {
			std::regex_search(comp, match, reg_regex);
			comp = match.suffix().str();
		}
		if (mode == 6) {
			std::regex_search(comp, match, reg_regex);
			comp = match.suffix().str();	
		}

		std::regex_search(comp, match, const_regex);
		std::string const_str = match.str();

		bool is_label = std::regex_match(const_str, label_regex);
		bool is_local = std::regex_match(const_str, local_regex);
		bool is_number = std::regex_match(const_str, number_regex);
		bool is_char = std::regex_match(const_str, char_regex);

		std::cout << "const: " << const_str << std::endl;
		std::cout << "lbl: " << is_label << " loc: " << is_local << std::endl;

		if (is_label) {
			std::string label = const_str;
			if (label_map.find(label) == label_map.end())
				throw EXCEPTION("label not defined: %s [line %d] %s",
						label.c_str(), instr->line_nr, instr->line.c_str());
			return label_map[label]->addr;
		}
		if (is_local) {
			std::string label = parrent_label + const_str;
			if (label_map.find(label) == label_map.end())
				throw EXCEPTION("local label not defined: %s [line %d] %s",
						label.c_str(), instr->line_nr, instr->line.c_str());
			return label_map[label]->addr;
		}
		if (is_local) {
			std::string label = parrent_label + const_str;
			if (label_map.find(label) == label_map.end())
				throw EXCEPTION("local label not defined: %s [line %d] %s",
						label.c_str(), instr->line_nr, instr->line.c_str());
			return label_map[label]->addr;
		}
		if (is_number) {
			return read_num(const_str, instr);
		}
		if (is_char) {
			if (const_str.size() != 3)
				throw EXCEPTION("invalid character: %s [line %d] %s",
						const_str.c_str(), instr->line_nr, instr->line.c_str());
			return const_str[1];
		}
		return 0;
	}

	uint32_t read_num(std::string num_str, AsmInstr *instr) {
		try {
			return own_stol(num_str);
		}
		catch (std::exception& ex) {
			throw EXCEPTION("parsing number err: %s [line %d] %s",
					ex.what(), instr->line_nr, instr->line.c_str());
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
			if (reg_expr.find(key) != std::string::npos) {
				return true;
			}
		}
		return false;
	}

	bool any_unexpanded() {
		using namespace nlohmann;
		for (auto&& [key, val] : j_match["macro"].get<json::object_t>()) {
			if (needs_expansion(val.get<std::string>())) {
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
				if (needs_expansion(val.get<std::string>())) {
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
		while (any_unexpanded()) {
			for (auto&& [key, val] : j_match["macro"].get<json::object_t>()) {
				if (needs_expansion(val.get<std::string>())) {
					std::set<std::string> in_expansion;
					in_expansion.insert(key);
					expand_macro(key, in_expansion);
				}
			}
		}
		for (auto&& [key, data] : j_match.get<json::object_t>()) {
			if (data.is_string()) {
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