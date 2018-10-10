#ifndef VGA_H
#define VGA_H

#include <memory>
#include "obj_dir/Vvga.h"

struct VGA {
	enum {
		DISPLAY_NOW		= 0b0000'0000'0000'0001,
		WRITE_PIXEL		= 0b0000'0000'0000'0010
	};

	bool present = false;
	std::shared_ptr<Vvga> chip = std::shared_ptr<Vvga>(new Vvga);
	
	size_t width = 640;
	size_t height = 480;
	size_t size = width * height;
	std::shared_ptr<int[]> vmem = std::shared_ptr<int[]>(new int[size]);
	std::function<void(int, int, uint)> putpixel;
	std::function<void(void)> swapBuffers;
	std::function<void(void)> focus;

	VGA() {}
	VGA (std::function<void(int, int, uint)> putpixel,
			std::function<void(void)> swapBuffers,
			std::function<void(void)> focus)
	: putpixel(putpixel), swapBuffers(swapBuffers), focus(focus) {}

	void update (bool clk) {
		chip->clk = clk;
		chip->rst = 0;

		/* vga specific: */
		if (chip->ctrl & WRITE_PIXEL) {
			vmem[chip->addr] = chip->data;
			chip->ctrl &= ~WRITE_PIXEL;
		}
		if (chip->ctrl & DISPLAY_NOW) {
			focus();
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					putpixel(j, i, vmem[i * width + j]);

			swapBuffers();
			memset(vmem.get(), 0, size);
		}
		/* end vga specific */
		
		chip->eval();
	}
};

#endif