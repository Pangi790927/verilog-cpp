#ifndef BRIDGE_H
#define BRIDGE_H

#include <memory>
#include "obj_dir/Vbridge.h"

struct Bridge {
	bool present = false;
	std::shared_ptr<Vbridge> chip = std::shared_ptr<Vbridge>(new Vbridge);
	
	std::shared_ptr<char[]> mem = nullptr;
	size_t size = 0;

	Bridge() {}
	Bridge (size_t size) : size(size) {
		mem = std::shared_ptr<char[]>(new char[size]);
	}

	void update (bool clk) {
		chip->clk = clk;
		chip->rst = 0;
		
		/* bridge specific: */
		
		/* end bridge specific */
		
		chip->eval();
	}
};

#endif