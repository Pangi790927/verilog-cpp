#ifndef VGA_H
#define VGA_H

#include <memory>
#include "mobo.h"
#include "fonts.h"
#include "vga_que.h"

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
	static const size_t max_vga_que = 1000;
	static const size_t char_width = 8;
	static const size_t char_height = 14;
	static const size_t text_row_count = height / char_height;
	static const size_t text_col_count = width / char_width;

	Mobo &mobo;
	size_t size = width * height;
	std::shared_ptr<int[]> vmem = std::shared_ptr<int[]>(new int[size]);
	std::function<void(int, int, uint)> putpixel;
	VgaQue<max_vga_que> vga_que;

	std::future<void> vga_async_run;
	std::atomic<bool> done = false;
	std::mutex mu;

	VGA (Mobo &mobo, const std::function<void(int, int, uint)>& putpixel)
	: putpixel(putpixel), mobo(mobo)
	{
		fonts::load<char_height, char_width>(char_font);

		vga_async_run = std::async([&] {
			while (!done) {
				if (vga_que.size() > 0) {
					auto elem = vga_que.pop();
					insert_char(std::get<0>(elem), std::get<1>(elem));
				}
			}
		});
	}

	~VGA() {
		done = true;
		vga_async_run.get();
	}

	void refresh() {
		if (done) {
			return;
		}

		mobo.chip->vga_ctrl_from_hw = 0;

		if (mobo.chip->vga_ctrl_to_hw & VGA_OE) {
			mobo.chip->data_from_hw = vmem[mobo.chip->addr];
			mobo.chip->vga_ctrl_from_hw |= VGA_ACK;
		}

		if ((mobo.chip->vga_ctrl_to_hw & VGA_WE) && 
				vga_que.size() < max_vga_que)
		{
			vmem[mobo.chip->addr] = mobo.chip->data_to_hw; 
			vga_que.push(mobo.chip->data_to_hw, mobo.chip->addr);
			mobo.chip->vga_ctrl_from_hw |= VGA_ACK;
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

// h								t
// ---------------------------------
//                    p

// struct VgaQue {
// 	atomic<int> tail;
// 	atomic<int> head;
// 	atomic<int> len;
// 	std::vector<ceva> vec;

// 	VgaQue() {
// 		tail = 0;
// 		head = 0;
// 	}

// 	void push(uint color_char, int index) {
// 		vec[tail] = pixel(color_char, index);
// 		tail = (tail + 1) % max_size;
// 		len = len + 1;
// 	}

// 	pixel pop() {
// 		pixel aux = vec[head];
// 		head = (head + 1) % max_size;
// 		len = len - 1;
// 		return aux;
// 	}

// 	size_t size() {
// 		return len;
// 	}
// }

// {
// 	// vga
// 	if (size() > 0)
// 		new_pixel = pop();
// }

// {
// 	// mobo
// 	{
// 		// cpu
// 		if (cpu wants push)
// 			signal vga want push
// 			wait vga respond
// 	}
// 	if (step % (20 + size() / 10) == 0 && size() < max_size / 2) {
// 		// vga other side
// 		if (cpu wants push)
// 			push(pixel);
// 	}
// }

#endif
