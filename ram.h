#ifndef RAM_H
#define RAM_H

#include <memory>
#include "mobo.h"

struct RAM {
	std::shared_ptr<char[]> mem = nullptr;
	size_t size = 0;
	Mobo &mobo;

	RAM (Mobo &mobo, size_t size) : mobo(mobo), size(size) {
		mem = std::shared_ptr<char[]>(new char[size]);
	}

	std::atomic<bool> done = false;
	std::mutex mu;
	std::future<void> async_run = std::async([this] {
		while (!done) {
			std::lock_guard<std::mutex> guard(mobo.mu);
			// access mobo.chip->pins
		}
	});

	~RAM() {
		done = true;
		async_run.get();
	}
};

#endif