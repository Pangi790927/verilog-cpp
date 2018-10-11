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
	update_changed(a, b, old);										\
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

	template <typename Type>
	void update_changed (Type& a, Type& b, Type& original) {
		if (original != a) {
			if (original != b)
				std::cout << "both changed, not good" << std::endl;
			else {
				b = a;

				std::cout << "Printing bits of a: ";
				for (uint i = 0; i < 8 * sizeof(Type); ++i) {
					bool cba = (a & (1 << i));
					bool cbb = (b & (1 << i));
					bool cbo = (original & (1 << i));

					if (cba == 0 && cba != cbb) {

					} else if (cba != cbb) {

					}
					std::cout << (a & (1 << i)) << " ";
				}
				std::cout << std::endl;
			}
		}
		else if (original != b)
			a = b;
		original = a;
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

	void updateConnections() {

	}
};

#endif