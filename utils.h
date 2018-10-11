#ifndef UTILS_H
#define UTILS_H

#include <cxxabi.h>
#include <string>

namespace util {
	std::string demangle(const char* name) {
		int status = -4;
		std::unique_ptr<char, decltype(&std::free)> res {
			abi::__cxa_demangle(name, NULL, NULL, &status),
			&std::free
		};

		return (status==0) ? res.get() : name ;
	}

	template <typename Type>
	std::string str_type (Type&&) {
		return demangle(typeid(Type).name());
	}

	template <typename Type>
	std::string str_type () {
		return demangle(typeid(Type).name());
	}
}


#endif

