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

	//std::future<void> async_run;
	std::atomic<bool> done = false;
	std::mutex mu;

	RAM (Mobo &mobo, size_t size) : mobo(mobo), size(size) {
		mem = std::shared_ptr<char[]>(new char[size]);

		/*async_run = std::async([&] {
			while (!done) {
				// std::lock_guard<std::mutex> guard(mobo.mu);
				while (mobo.lock.test_and_set(std::memory_order_acquire))
					; // spin

				
				mobo.lock.clear(std::memory_order_release);
			}
		});*/
	}

	void refresh() {
		mobo.chip->ram_stat = 0;

		if (mobo.chip->ram_ctrl & RAM_OE) {
			mobo.chip->data_in = mem[mobo.chip->addr];
			mobo.chip->ram_stat |= RAM_ACK;
		}

		if (mobo.chip->ram_ctrl & RAM_WE) {
			mem[mobo.chip->addr] = mobo.chip->data_out;
			mobo.chip->ram_stat |= RAM_ACK;
		}

	}

	~RAM() {
		done = true;
		//async_run.get();
	}
};

#endif