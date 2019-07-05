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
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

static inline std::string trimComments (std::string line) {
	auto tokens = ssplit(line, ";");
	if (tokens.size() > 0)
		return tokens[0];
	return "";
}