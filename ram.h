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

	std::future<void> async_run;
	std::atomic<bool> done = false;
	std::mutex mu;

	RAM (Mobo &mobo, size_t size) : mobo(mobo), size(size) {
		mem = std::shared_ptr<char[]>(new char[size]);

		async_run = std::async([&] {
			while (!done) {
				// std::lock_guard<std::mutex> guard(mobo.mu);
				while (mobo.lock.test_and_set(std::memory_order_acquire))
					; // spin

				mobo.chip->ram_ctrl_from_hw = 0;

				if (mobo.chip->ram_ctrl_to_hw & RAM_OE) {
					mobo.chip->data_from_hw = mem[mobo.chip->addr];
					mobo.chip->ram_ctrl_from_hw |= RAM_ACK;
				}

				if (mobo.chip->ram_ctrl_to_hw & RAM_WE) {
					mem[mobo.chip->addr] = mobo.chip->data_to_hw;
					mobo.chip->ram_ctrl_from_hw |= RAM_ACK;
				}

				mobo.lock.clear(std::memory_order_release);
			}
		});
	}

	~RAM() {
		done = true;
		async_run.get();
	}
};

#endif