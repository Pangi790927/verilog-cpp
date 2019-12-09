#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

#define EXCEPTION(fmt, ...) \
	std::runtime_error([&](const char *func){\
		char buff[1024] = {0};\
		sprintf(buff, "[file: %s][func: %s][code_line: %d] " fmt,\
				__FILE__, func, __LINE__, ##__VA_ARGS__);\
		return std::string(buff);\
	}(__func__).c_str());

#endif