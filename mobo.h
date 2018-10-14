#ifndef MOBO_H
#define MOBO_H

#include "bridge.h"
#include "cpu.h"
#include "ram.h"
#include "vga.h"
#include "utils.h"

#define ADD_UPDATE_RULE(a, b)										\
{																	\
	static decltype(a) old = b = a;									\
	if (typeid(a) != typeid(b))										\
		std::cout << "type mismatch in update rule" << std::endl;	\
	old = a & ~old & ~b | b & a | ~old & b;							\
	a = b = old;													\
}

struct Mobo {
	std::vector<std::function<void(void)>> links;
	Bridge bridge;

	CPU cpu;
	RAM ram;
	VGA vga;

	bool clk_front = false;

	Mobo() {
		// bridge is part of mobo
		bridge.present = true;
	}

	void insertCpu (const CPU& cpu) {
		this->cpu = cpu;
		this->cpu.present = true;

		links.push_back([&] {
			ADD_UPDATE_RULE(bridge.chip->cpu_iodata, cpu.chip->iodata);
			ADD_UPDATE_RULE(bridge.chip->cpu_ioaddr, cpu.chip->ioaddr);
			ADD_UPDATE_RULE(bridge.chip->cpu_addr, cpu.chip->addr);
			ADD_UPDATE_RULE(bridge.chip->cpu_data, cpu.chip->data);
			ADD_UPDATE_RULE(bridge.chip->cpu_ctrl, cpu.chip->ctrl);
		});
	}

	void insertRam (const RAM& ram) {
		this->ram = ram;
		this->ram.present = true;
		links.push_back([&] {
			ADD_UPDATE_RULE(bridge.chip->ram_addr, ram.chip->addr);
			ADD_UPDATE_RULE(bridge.chip->ram_data, ram.chip->data);
			ADD_UPDATE_RULE(bridge.chip->ram_ctrl, ram.chip->ctrl);
		});
	}

	void initVga (const VGA& vga) {
		this->vga = vga;
		this->vga.present = true;
		links.push_back([&] {
			ADD_UPDATE_RULE(bridge.chip->vga_addr, vga.chip->addr);
			ADD_UPDATE_RULE(bridge.chip->vga_data, vga.chip->data);
			ADD_UPDATE_RULE(bridge.chip->vga_ctrl, vga.chip->ctrl);
		});
	}

	void update() {
		clk_front = !clk_front;

		bridge.update(clk_front);
		cpu.update(clk_front);
		ram.update(clk_front);
		vga.update(clk_front);

		for (auto&& link : links)
			link();	// propagates respective signal(s)
	}
};

/*
	a better solution for wiring cables:
	let's take an example (brg stands for bridge):
	old: 0110 1010
	cpu: 1010 0110
	brg: 0101 1000

	so the pins that changed here are (x signifies no change)
	cpu: 10xx 01xx
	brg: xx01 xx0x

	as we can see, we have might not have simultaneous changes, that
	meaning from our observation the bridge and the cpu don't enforce
	the same pins at the same time. We might have the cpu enforcing
	pin 0 on low and brg enforccing pin 0 on high but that is obviously a
	bug and we will not take it into consideration.

	now to create a mask of the changes we can xor the new values with the
	old ones:

	old: 0110 1010 ^
	cpu: 1010 0110
	--------------
		 1100 1100

	old: 0110 1010 ^
	brg: 0101 1001
	--------------
		 0011 0011

	so to retain only what changed by the cpu we can do (cpu ^ old) & cpu and
	to retain only what changed from the bridge we can do (brg ^ old) & brg.
	finally to get what didn't change we can (cpu ^ old | brg ^ old) & old.

	we get a final equation for the modifications on the wire:
	((cpu ^ old) & cpu) | ((brg ^ old) & brg) | (~(cpu ^ old | brg ^ old) & old)

	we have only 3 variables and we can see that this formula is invariant to
	the number of pins of the variables (each need to have the same number of
	pins but the actual number does not matter).

	we can create the following table:
	old cpu brg formula
	0	0	0		0
	0	0	1		1
	0	1	0		1
	0	1	1		x	(this case is degenerate but it would result in 1)
	1	0	0		x	(this case is degenerate but it would result in 0)
	1	0	1		0
	1	1	0		0
	1	1	1		1

	so now we build the following table:
		old cpu :	00	01	11	10
		brg		:0	0	1	0	0
				:1	1	1	1	0

	considering degenerate cases (we can see it does not help us):
		old cpu :	00	01	11	10
		brg		:0	0	1	0	x
				:1	1	x	1	0

	so now we can build the new formula:
	new = brg & ~old & ~cpu | cpu & brg | ~old & cpu;
*/
#endif