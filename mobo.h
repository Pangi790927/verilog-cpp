#ifndef MOBO_H
#define MOBO_H

#include "utils.h"
#include <future>
#include "obj_dir/Vmobo.h"

struct Mobo {
	std::shared_ptr<Vmobo> chip = std::shared_ptr<Vmobo>(new Vmobo);
	std::atomic<bool> done = false;
	std::mutex mu;

	std::future<void> async_run = std::async([this] {
		while (!done) {
			std::lock_guard<std::mutex> guard(mu);
			chip->eval();
		}
	});

	~Mobo() {
		done = true;
		async_run.get();
	}
};

#endif