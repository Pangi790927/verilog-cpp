#ifndef RAM_H
#define RAM_H

#include <memory>
#include "mobo.h"

#define RAM_IN_OE 1
#define RAM_IN_WE 2

#define RAM_OUT_ACK 1

struct RAM {
	Mobo &mobo;
	std::shared_ptr<char[]> mem = nullptr;
	size_t size = 0;

	RAM (Mobo &mobo, size_t size) : mobo(mobo), size(size) {
		mem = std::shared_ptr<char[]>(new char[size]);
	}

	std::atomic<bool> done = false;
	std::mutex mu;

	std::future<void> async_run = std::async([this] {
		while (!done) {
			std::lock_guard<std::mutex> guard(mobo.mu);
			mobo.chip->ram_ctrl_out = 0;

			if (mobo.chip->ram_ctrl_in & RAM_IN_OE) {
				mobo.chip->data_in = mem[mobo.chip->addr];
				mobo.chip->ram_ctrl_out |= RAM_OUT_ACK;
				std::cout << mobo.chip->addr << std::endl;
			}
		}
	});

	~RAM() {
		done = true;
		async_run.get();
	}
};

#endif