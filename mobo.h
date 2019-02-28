#ifndef MOBO_H
#define MOBO_H

#include "utils.h"
#include <future>
#include <chrono>
#include "obj_dir/Vmobo.h"

struct Mobo {
	std::shared_ptr<Vmobo> chip = std::shared_ptr<Vmobo>(new Vmobo);
	std::atomic<bool> done = false;
	std::mutex mu;
	std::atomic_flag lock = ATOMIC_FLAG_INIT;

	bool clk = true;

	// std::future<void> async_run = std::async([this] {
	// 	const clock_t begin_time = clock();

	// 	while (!done) {
	// 		// std::lock_guard<std::mutex> guard(mu);
	// 		while (lock.test_and_set(std::memory_order_acquire))  // acquire lock
	// 			; // spin
	// 		// uite de asta l-am pus pe asta aici
	// 		// ar dura mai putin? 
	// 		// dar parca tot lent e 
	// 		// si nici nu mai stiu daca mergea ...
	// 		i++;
			
			
	// 		lock.clear(std::memory_order_release);
	// 	}
		
	// });

	void refresh() {
		chip->clk = clk = !clk;
		chip->eval();
	}

	~Mobo() {
		done = true;
		//async_run.get();
	}
};

#endif