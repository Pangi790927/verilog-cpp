#ifndef RAM_H
#define RAM_H

#include <memory>
#include "obj_dir/Vram.h"

struct RAM {
	bool present = false;
	std::shared_ptr<Vram> chip = std::shared_ptr<Vram>(new Vram);
	
	std::shared_ptr<char[]> mem = nullptr;
	size_t size = 0;

	RAM() {}
	RAM (size_t size) : size(size) {
		mem = std::shared_ptr<char[]>(new char[size]);
	}

	void update (bool clk) {
		chip->clk = clk;
		chip->rst = 0;
		
		/* ram specific: */

		/* end ram specific */
		
		chip->eval();
	}
};

#endif