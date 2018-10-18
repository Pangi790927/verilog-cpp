#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <verilated.h>
#include "obj_dir/Vcpu.h"
#include "OpenglWindow.h"
#include "mobo.h"
#include "gl_screen.h"

/* Current simulation time */
vluint64_t main_time = 0; 

/* exported function to verilog ($time) */
double sc_time_stamp () {
    return main_time;
}

// still need:
// memory
// hdd
// keyboard
// display
// timer

// optional:
// mouse
// network card
// usb (far away)

// sugested memory work scenario for read:
// * cpu sets addr, sets control on read and query mem
// * on next iteration, bridge sees control changed so it sets ram control on
//   read memory and query
// * next iteration ram sees query, puts ctrl working and asks phys computer for
//   ram.
// * After given memory data and seeing ctrl working freed ram puts the received
//   data on output and signals by unsetting query flag
// * bridge finally sees change in ctrl and gives data from ram to cpu and
//   releases cpu flag
// * cpu finaly sees his query flag up and removes read flag and continues his
//   job
// 
// work scenario for write is very similar, the difference is that the cpu
// writes what it wants in ram in it's data register and sets write flag instead
// of read flag
//
// working with vga (THIS IS NO LONGER THE TARGETED BEHAVIOR):
// * cpu writes in vga memory instad of ram (mmap and bridge will do the job)
// * after many writes cpu writes with iomap
// * bridge sees io register changed so it propagates the change to the specific
//   device (vga in our case)
// * next iteration vga sees the change and signals phys to draw display by
//   changing specific control
// * phys will change display and put the signal on 0
// * vga will see and it will put 0 on user signal
// * till now cpu, could pool the bit that he used in iomap to see when screen
//   was flashed, but it is not necesary
// 
// bridge really is useless in iomap, cpu could simply 'talk' to other devices
// for example in the vga case:
// * cpu would put address in 

int main(int argc, char const *argv[]) {
	Verilated::commandArgs(argc, argv);

	float window_scale = 640. / 640;
	std::atomic<bool> io_close = false;
	util::SyncCond io_sync;
	util::SyncCond gl_sync;

	GlScreen *pgl_screen;
	std::mutex screen_mu;

	auto io_thread = std::thread([&](){
		OpenglWindow display(VGA::width * window_scale,
				VGA::height * window_scale, "VGA Display");
		
		glEnable(GL_TEXTURE_2D);
		GlScreen gl_screen(VGA::width, VGA::height);
		pgl_screen = &gl_screen;

		io_sync.notify();

		while (display.active) {
			if(display.handleInput())
				if (display.keyboard.getKeyState(display.keyboard.ESC))
					display.requestClose();
				// if (other) put in cirular buffer and raise interrupt
			display.focus();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// a pitfall here is that this function redraws the screen
			// asyncroniously, we should be signaled to redraw, else just
			// wait for a time
			{
				std::lock_guard<std::mutex> guard(screen_mu);
				gl_screen.render();
			}

			display.swapBuffers();
		}
		io_close = true;
		io_sync.wait();
	});

	io_sync.wait();

	Mobo mobo;
	CPU cpu;
	RAM ram(1 << 20);	// 1Mb ram
	VGA vga([&] (int xi, int yi, int color) {
		std::lock_guard<std::mutex> guard(screen_mu);
		pgl_screen->put_pixel(color, xi, yi);
	});

	mobo.insertCpu(cpu);
	mobo.insertRam(ram);
	mobo.initVga(vga);

	volatile int x = 0;
	auto profiler = std::thread([&]{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << x / 1000'000. << " MHz" << std::endl;
	});

	while (!io_close) {
		// those inserts here are verry slow, they bring the cpu from 25Mhz to
		// 0.0005Mhz (500Hz) so don't use them when not needed
		for (int i = 0; i < 16; i++)
			for (int j = 0; j < 16; j++)
				vga.insert_char(((i << 16) & 0xff0000) + ((j << 8) & 0xff00)
						+ 0, i * VGA::text_col_count + j);

		for (int i = 0; i < 256; i++)
			vga.insert_char(0x0000'0f'00 + i, i + 16 * VGA::text_col_count);

		vga.insert_char(0x0000'0f'00 + 0, 80 * 25 - 1);
		x++;
		mobo.update();
	}

	io_sync.notify();

	if (profiler.joinable())
		profiler.join();
	if (io_thread.joinable())
		io_thread.join();

	return 0;
}
