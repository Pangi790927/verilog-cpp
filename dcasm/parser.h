#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "str_helper.h"
#include "instr_map.h"
#include "asm_instr.h"
#include "debug.h"

struct Parser {
	std::vector<Instr *> asmInstr;
	std::string last_label;
	std::ifstream in;
	std::ofstream out;

	Parser (std::string fileIn, std::string fileOut)
	: in(fileIn.c_str()), out(fileOut.c_str())
	{
		if (!in.good())
			throw EXCEPTION("could not open input file");
		if (!out.good())
			throw EXCEPTION("could not open output file");
	}

	void parse() {
		std::string line;
		int line_cnt = 1;

		std::cout << "============= PARSING BEGIN =============" << std::endl;
		while (getline(in, line)) {
			parseLine(line, line_cnt++);
		}
		std::cout << "============== PARSING END ==============" << std::endl;

		splitInstructions();
	}

	void splitInstructions() {
		std::cout << "============== SPLIT BEGIN ==============" << std::endl;

		for (auto &rawInstr : asmInstr) {
			if (auto label = dynamic_cast<LabelInstr*>(rawInstr)){
				if (label->parrent.compare("") == 0) {
					std::cout << "[global label] " << label->label << std::endl;
				}
				else {
					std::cout << "[local label] " << label->label << std::endl;
				}
			}

			if (auto instr = dynamic_cast<AsmInstr*>(rawInstr)){
				std::cout << "[instr] " << instr->instr << "\t" << instr->args << std::endl;
			}
		}

		std::cout << "=============== SPLIT END ===============" << std::endl;
	}

	void parseLine(std::string &line, int line_cnt) {
	    std::string trimmed = trim(line);
	    std::string instr = trimComments(trimmed);

	    if (isLabel(instr)) {
	    	if (isLocalLabel(instr)) {
	    		if (last_label == "")
	    			throw EXCEPTION("No label for local label, [at line: %d]", line_cnt);
	    		asmInstr.push_back(new LabelInstr(line_cnt,
	    				extractLabel(instr), last_label));
	    	}
	    	else {
	    		last_label = extractLabel(instr);
	    		asmInstr.push_back(new LabelInstr(line_cnt, last_label));
	    	}

	    	return ;
	    }

	    auto tokens = ssplit(instr, " \t");
	    std::string command;

	    if (tokens.size() == 0) {
	    	return ;
	    }
	    command = tokens[0];
	    if (trim(command).size() == 0) {
	    	return ;
	    }

	    auto it = instr_map.find(command);

	    if (it == instr_map.end()) {
	        std::cerr << line << std::endl;
	        throw EXCEPTION("Not a valid instruction: %s [at line: %d]",
	        		line.c_str(), line_cnt);
		}

	    std::string args;
	    for (int i = 1; i < tokens.size(); i++)
	    	args += tokens[i];

	    std::cout << "split instr: " << command << "$ " << args << "$" << std::endl;
		asmInstr.push_back(new AsmInstr(line_cnt, command, args));
	}

	std::string extractLabel(std::string instr) {
		std::string trimed_instr = trim(instr);
		auto tok_instr = ssplit(trimed_instr, " \t");
		if (isLocalLabel(trim(tok_instr[0]))) {
			return trim(tok_instr[0]).substr(1);
		}
		else {
			trim(tok_instr[0]);
		}
	}

	bool isLocalLabel(std::string instr) {
		if (instr.size() == 0)
			return false;
		return instr[0] == '.';
	}

	bool isLabel(std::string instr) {
		std::string trimed_instr = trim(instr);
		auto tok_instr = ssplit(trimed_instr, " \t");
		std::string label_part;
		if (tok_instr.size() > 1) {
			if (tok_instr.size() == 2) {
				if (trim(tok_instr[1]) != ":")
					return false;
				label_part = tok_instr[0];
			}
			else {
				return false;
			}
		}
		if (label_part.size() == 0) {
			if (trimed_instr.size() == 0)
				return false;
			if (trimed_instr[trimed_instr.size() - 1] != ':')
				return false;
			label_part = trimed_instr.substr(0, trimed_instr.size() - 1);
		}
		if (label_part.size() == 0)
			return false;
		if (isdigit(label_part[0]))
			return false;
		if (label_part[0] == '.') {
			label_part = label_part.substr(1);
		}
		std::string accepted_chars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_1234567890";
		for (auto&& chr : label_part)
			if (accepted_chars.find(chr) == std::string::npos)
				return false;
		return true;
	}
};

#endif