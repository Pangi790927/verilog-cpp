#ifndef STR_HELPER_H
#define STR_HELPER_H

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

static inline std::vector<std::string> ssplit(const std::string& str,
		const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t last = 0;
    size_t next = 0;
    while ((next = str.find_first_of(delim, last)) != std::string::npos) {
    	tokens.push_back(str.substr(last, next - last));
    	last = next + 1;
    }
    tokens.push_back(str.substr(last));

    std::vector<std::string> ret;
    for (int i = 0; i < tokens.size(); i++)
    	if (!tokens[i].empty() || i == 0 || i == tokens.size() - 1)
    		ret.push_back(tokens[i]);
    return ret;
}


// trim from start
static inline std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

static inline std::string trimComments (std::string line) {
	auto tokens = ssplit(line, ";");
	if (tokens.size() > 0)
		return tokens[0];
	return "";
}

static inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if(from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

static inline uint32_t own_stol(std::string num_str) {
	if (num_str.size() > 1) {
		if (num_str[0] == '0' && num_str[1] == 'b')
			return std::stol(num_str.c_str() + 2, NULL, 2);
	}
	return std::stol(num_str, NULL, 0);
}

template <typename Arg>
static auto sformat_arg(Arg&& arg) {
	return std::forward<Arg>(arg);
}

template <>
auto sformat_arg <std::string&> (std::string& str) {
	return str.c_str();
}

template <>
auto sformat_arg <const std::string&> (const std::string& str) {
	return str.c_str();
}

template <typename ...Types>
static std::string sformat(const char* fmt, Types ...args) {
	int len = snprintf(NULL, 0, fmt, sformat_arg(args)...);
	if (len <= 0) {
		return "";	
	}
	std::vector<char> vec(len + 1);
	snprintf(&vec[0], vec.size(), fmt, sformat_arg(args)...);
	return &vec[0];
}


#endif