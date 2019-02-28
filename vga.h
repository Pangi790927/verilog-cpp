#ifndef VGA_H
#define VGA_H

#include <memory>
#include "mobo.h"
#include "fonts.h"

#define VGA_OE 1
#define VGA_WE 2

#define VGA_ACK 1

struct VGA {
	enum {
		HW_WRITE		= 0b0000'0000'0000'0100,
		TEXT_MODE		= 0b0000'0000'0000'1000,	// 1 for text, 0 for graphic
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
	// std::shared_ptr<Vvga> chip = std::shared_ptr<Vvga>(new Vvga);
	
	static const size_t width = 640;
	static const size_t height = 350;
	static const size_t char_width = 8;
	static const size_t char_height = 14;
	static const size_t text_row_count = height / char_height;
	static const size_t text_col_count = width / char_width;

	Mobo &mobo;
	size_t size = width * height;
	std::shared_ptr<int[]> vmem = std::shared_ptr<int[]>(new int[size]);
	std::function<void(int, int, uint)> putpixel;

	std::future<void> async_run;
	std::atomic<bool> done = false;
	std::mutex mu;

	VGA (Mobo &mobo, const std::function<void(int, int, uint)>& putpixel)
	: putpixel(putpixel), mobo(mobo)
	{
		fonts::load<char_height, char_width>(char_font);

		/*async_run = std::async([&] {
			while (!done) {
				std::lock_guard<std::mutex> guard(mobo.mu);
				// while (mobo.lock.test_and_set(std::memory_order_acquire))
				// 	; // spin

				

				// mobo.lock.clear(std::memory_order_release);
			}
		});*/
	}

	~VGA() {
		done = true;
		async_run.get();
	}

	void refresh() {
		if (done) {
			return;
		}

		mobo.chip->vga_ctrl_from_hw = 0;

		//std::cout << "VGA loop" << std::endl;


		if (mobo.chip->vga_ctrl_to_hw & VGA_OE) {
			mobo.chip->data_from_hw = vmem[mobo.chip->addr];
			mobo.chip->vga_ctrl_from_hw |= VGA_ACK;

			//std::cout << "Someone is reading" << std::endl;
		}

		if (mobo.chip->vga_ctrl_to_hw & VGA_WE) {
			vmem[mobo.chip->addr] = mobo.chip->data_to_hw; 
			insert_char(mobo.chip->data_to_hw, mobo.chip->addr);
			mobo.chip->vga_ctrl_from_hw |= VGA_ACK;
			//std::cout << "Someone is writing" << std::endl;
		}
	}

	/* a character will be displayed inside a 20:8 pixels*/
	void insert_char (uint color_char, int index) {
		int x = (index % text_col_count) * char_width;
		int y = (index / text_col_count) * char_height;

		put_char_at(color_char, x, y);
	}

	// color_char: 0x00'bg'fg'ch;
	void put_char_at (uint color_char, int x, int y) {
		uint to_print = color_char & 0xff;
		uint fg_color = colors[(color_char & 0xff00) >> 8];
		uint bg_color = colors[(color_char & 0xff0000) >> 16];

		for (int i = 0; i < char_height; i++)
			for (int j = 0; j < char_width; j++)
				if (char_font[to_print][i][j])
					putpixel(j + x, i + y, fg_color);
				else
					putpixel(j + x, i + y, bg_color);
	}

	bool char_font[256][char_height][char_width] = {
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
		}
	};
	
	/* those colors are abgr not rgba reason is that most semnificant
		bytes are red */
	uint colors[256] = {
		0x00'00'00'00, // black
		0x00'aa'00'00, // blue
		0x00'00'aa'00, // green
		0x00'aa'aa'00, // cyan
		0x00'00'00'aa, // red
		0x00'aa'00'aa, // magenta
		0x00'aa'55'aa, // brown 
		0x00'aa'aa'aa, // light gray 
		0x00'55'55'55, // dark gray
		0x00'ff'55'55, // light blue
		0x00'55'ff'55, // light green
		0x00'ff'ff'55, // light cyan
		0x00'55'55'ff, // light red
		0x00'ff'55'ff, // light magenta
		0x00'55'ff'ff, // yellow
		0x00'ff'ff'ff  // white
	};
};

#endif
