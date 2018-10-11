#ifndef VGA_H
#define VGA_H

#include <memory>
#include "obj_dir/Vvga.h"
#include "fonts.h"

struct VGA {
	enum {
		DISPLAY_NOW		= 0b0000'0000'0000'0001,
		WRITE_PIXEL		= 0b0000'0000'0000'0010,
		TEXT_MODE		= 0b0000'0000'0000'0100,	// 1 for text, 0 for graphic
		CLEAR_BIT		= 0b0000'0000'0000'1000
	};

	enum {
		Black			= 0,
		Blue			= 1,
		Green			= 2,
		Cyan			= 3,
		Red				= 4,
		Magenta			= 5,
		Brown			= 6,
		Light_Gray		= 7,
		Dark_Gray		= 8,
		Light_Blue		= 9,
		Light_Green		= 10,
		Light_Cyan		= 11,
		Light_Red		= 12,
		Light_Magenta	= 13,
		Yellow			= 14,
		White			= 15
	};

	bool present = false;
	std::shared_ptr<Vvga> chip = std::shared_ptr<Vvga>(new Vvga);
	
	static const size_t width = 640;
	static const size_t height = 350;
	static const size_t char_width = 8;
	static const size_t char_height = 14;

	size_t size = width * height;
	std::shared_ptr<int[]> vmem = std::shared_ptr<int[]>(new int[size]);
	std::function<void(int, int, uint)> putpixel;
	std::function<void(void)> swapBuffers;
	std::function<void(void)> focus;

	VGA() {}
	VGA (const std::function<void(int, int, uint)>& putpixel,
			const std::function<void(void)>& swapBuffers,
			const std::function<void(void)>& focus)
	: putpixel(putpixel), swapBuffers(swapBuffers), focus(focus) {
		fonts::test();
	}

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
			if (chip->ctrl & TEXT_MODE) {
				display_text();
			}
			else
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
						putpixel(j, i, vmem[i * width + j]);

			swapBuffers();
			chip->ctrl &= ~DISPLAY_NOW;
		}
		if (chip->ctrl & CLEAR_BIT) {
			focus();
			memset(vmem.get(), 0, size);
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					putpixel(j, i, vmem[i * width + j]);
			swapBuffers();
			chip->ctrl &= ~CLEAR_BIT;
		}
		/* end vga specific */
		
		chip->eval();
	}

	/* a character will be displayed inside a 20:8 pixels*/
	void display_text() {
		for (int i = 0; i < height / char_height; i++)
			for (int j = 0; j < width / char_width; j++)
				put_char_at(vmem[i * width / char_width + j],
						i * char_height, j * char_width);
	}

	void put_char_at (uint color_char, int x, int y) {
		char to_print = color_char & 0xff;
		uint fg_color = colors[(color_char & 0xff00) >> 8];
		uint bg_color = colors[(color_char & 0xff0000) >> 16];

		for (int i = 0; i < char_height; i++)
			for (int j = 0; j < char_height; j++)
				if (char_display[to_print][i][j])
					putpixel(j + y, i + x, fg_color);
				else
					putpixel(j + y, i + x, bg_color);
	}

	bool char_display[256][char_height][char_width] = {
		{
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 1, 1, 1, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 1, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 1, 1, 1, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 1, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0}
		},
		{
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 1, 1, 1, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 1, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 1},
			{0, 0, 0, 1, 1, 0, 0, 1},
			{0, 0, 0, 1, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 0, 0, 1},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0}
		}
	};
	
	/* those colors are abgr not rgba reason is that */
	uint colors[256] = {
		0x0000'0000, // black
		0x00ff'0000, // blue
		0x0000'8000, // green
		0x00ff'ff00, // cyan
		0x0000'0080, // red
		0x0080'0080, // magenta
		0x002A'2AA5, // brown 
		0x00D3'D3D3, // light gray 

		0x00A9'A9A9, // dark gray
		0x00e6'D8Ad, // light blue
		0x0000'ff00, // light green
		0x00ff'ffe0, // light cyan
		0x0000'00ff, // light red
		0x00ff'00ff, // light magenta
		0x0000'ffff, // yellow
		0x00ff'ffff  // white
	};
};

#endif
