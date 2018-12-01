#ifndef RAM_H
#define RAM_H

#include <memory>
#include "mobo.h"

#define RAM_OE 1
#define RAM_WE 2

#define RAM_ACK 1

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
			mobo.chip->ram_ctrl_from_hw = 0;

			if (mobo.chip->ram_ctrl_to_hw & RAM_OE) {
				mobo.chip->data_from_hw = mem[mobo.chip->addr];
				mobo.chip->ram_ctrl_from_hw |= RAM_ACK;
				std::cout << mobo.chip->addr << std::endl;
			}

			if (mobo.chip->ram_ctrl_to_hw & RAM_WE) {
				mem[mobo.chip->addr] = mobo.chip->data_to_hw;
				mobo.chip->ram_ctrl_from_hw |= RAM_OUT_ACK;
				std::cout << mobo.chip->addr << " " << mobo.chip->data_to_hw << std::endl;
			}
		}
	});

	~RAM() {
		done = true;
		async_run.get();
	}
};

#endif