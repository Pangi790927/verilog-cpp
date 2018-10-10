#ifndef CPU_H
#define CPU_H

#include <memory>
#include "obj_dir/Vcpu.h"

struct CPU {
	bool present = false;
	std::shared_ptr<Vcpu> chip = std::unique_ptr<Vcpu>(new Vcpu);

	void update (bool clk) {
		chip->clk = clk;
		chip->rst = 0;
		chip->eval();
	}
};

#endif