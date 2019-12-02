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
#include "eeprom.h"
#include "TestClass.h"

/* Current simulation time */
vluint64_t main_time = 0; 

/* exported function to verilog ($time) */
double sc_time_stamp () {
    return main_time;
}

int main(int argc, char const *argv[]) {
	Verilated::commandArgs(argc, argv);

	float window_scale = 640. / 640;
	util::SyncCond io_init;
	util::SyncCond io_close;
	util::SyncCond use_io_done;

	GlScreen *pgl_screen;
	std::mutex screen_mu;

	bool done = false;

	auto io_thread = std::thread([&](){
		OpenglWindow display(VGA::width * window_scale,
				VGA::height * window_scale, "VGA Display");
		
		glEnable(GL_TEXTURE_2D);
		GlScreen gl_screen(VGA::width, VGA::height);
		pgl_screen = &gl_screen;

		io_init.notify();

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

		done = true;

		io_close.notify();
		use_io_done.wait();
	});

	std::cout << "----- Start -----" << std::endl;
	{
		Mobo mobo;
		RAM ram(mobo, 1 << 20);	// 1Mb ram

		std::string bios_path = argc > 1 ? std::string(argv[1]) : "./dcasm/bios.bin";
		EEPROM(ram, bios_path);
		TestClass::dummy(-1);

		io_init.wait();
		VGA vga(
			mobo,
			[&] (int xi, int yi, int color) {
				std::lock_guard<std::mutex> guard(screen_mu);
				pgl_screen->put_pixel(color, xi, yi);
			}
		);

		int tick = 0;
		const clock_t begin_time = clock();
		
		while (!done) {
			mobo.refresh();
			if (tick % 3 == 0) {
				ram.refresh();
			}

			if (tick % 20 == 0) {
				vga.refresh();
			}

			tick++;
		}

		std::cout << float(tick) / (float(clock() - begin_time)
				/ CLOCKS_PER_SEC) / 1000'000. << "MHz" << std::endl;

		io_close.wait();
	}
	std::cout << "closing ..." << std::endl;

	use_io_done.notify();
	if (io_thread.joinable())
		io_thread.join();

	return 0;
}


// // uite
// int i = 0;
// while (true) {
// 	cpu.eval()
// 	if (i % 10 == 0) {
// 		ram.eval();
// 	}
// 	if (i % 1000 == 0) {
// 		vga.eval();
// 	}
// 	i++
// }

// problema e ca vga mananca mult timp

// adica ar fi ceva de genu

// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// ram
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// ram
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// ram
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// ram
// ...
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu
// cpu 0.1ms
// cpu 0.1ms
// ram 2ms
// vga 100ms

// le-am luat din burta
// dar ceva de genu?

// si problema e ca 100ms alea cpu-ul 
// dar si cu arhitectura asta, dureaza mult sa luam mutexuri
// ca vrem sa avem mhz pe cpu
// si mutexurile sunt foarte lente
// sunt asemanatoare cu printf