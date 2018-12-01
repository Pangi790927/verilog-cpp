#ifndef MOBO_H
#define MOBO_H

#include "utils.h"
#include <future>
#include "obj_dir/Vmobo.h"

struct Mobo {
	std::shared_ptr<Vmobo> chip = std::shared_ptr<Vmobo>(new Vmobo);
	std::atomic<bool> done = false;
	std::mutex mu;
	std::atomic_flag lock = ATOMIC_FLAG_INIT;

	std::future<void> async_run = std::async([this] {
		bool clk = true;
		while (!done) {
			std::lock_guard<std::mutex> guard(mu);
			// while (lock.test_and_set(std::memory_order_acquire))  // acquire lock
			// 	; // spin

			chip->clk = clk = !clk;
			chip->eval();
			
			// lock.clear(std::memory_order_release);
		}
	});

	~Mobo() {
		done = true;
		async_run.get();
	}
};

#endif