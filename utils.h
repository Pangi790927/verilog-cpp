#ifndef UTILS_H
#define UTILS_H

#include <cxxabi.h>
#include <string>
#include <sstream>
#include <mutex>
#include <condition_variable>

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

	struct SyncCond {
		std::mutex mu;
		bool done_wait = false;
		std::condition_variable cond;

		void wait() {
			std::unique_lock<std::mutex> guard(mu);
			cond.wait(guard, [&]{ return done_wait; });
			done_wait = false;
		}

		void notify() {
			{
				std::lock_guard<std::mutex> guard(mu);
				done_wait = true;
			}
			cond.notify_one();
		}
	};

	template <typename Type>
	std::string bin_str (Type a) {
		std::stringstream ss;
		for (int i = sizeof(a) * 8 - 1; i >= 0 ; i--)
			ss << bool(a & (1 << i));
		return ss.str();
	}
}


#endif

