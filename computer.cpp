#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <verilated.h>
#include "OpenglWindow.h"
#include "mobo.h"
#include "gl_screen.h"
#include "vga.h"
#include "ram.h"

/* Current simulation time */
vluint64_t main_time = 0; 

/* exported function to verilog ($time) */
double sc_time_stamp () {
    return main_time;
}

int main(int argc, char const *argv[]) {
	Verilated::commandArgs(argc, argv);

	float window_scale = 640. / 640;
	util::SyncCond io_sync;
	util::SyncCond io_close;

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
		io_close.notify();
		io_sync.wait();
	});

	std::cout << "----- Start -----" << std::endl;
	{
		Mobo mobo;
		RAM ram(mobo, 1 << 20);	// 1Mb ram

		io_sync.wait();
		// VGA vga(
		// 	mobo,
		// 	put_pixel
		// );
		
		io_close.wait();
	}

	io_sync.notify();
	if (io_thread.joinable())
		io_thread.join();

	return 0;
}
