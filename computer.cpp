#include <iostream>
#include <thread>
#include <verilated.h>
#include "obj_dir/Vcpu.h"
#include "OpenglWindow.h"
#include "mobo.h"

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
// working with vga:
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
	float window_scale = 1.5;
	OpenglWindow display(640 * window_scale, 350 * window_scale, "VGA Display");

	Mobo mobo;
	CPU cpu;
	RAM ram(1 << 20);	// 1Mb ram
	VGA vga([&] (int xi, int yi, int color) {
		float r = (((0x000000ff & color) >> 0 ) & 0xff) / float(256); 
		float g = (((0x0000ff00 & color) >> 8 ) & 0xff) / float(256); 
		float b = (((0x00ff0000 & color) >> 16) & 0xff) / float(256); 
		
		float side_x = display.width / float(vga.width);
		float side_y = display.height / float(vga.height);

		float x = xi * side_x;
		float y = display.height - yi * side_y;

		glBegin(GL_QUADS);
			glColor3f(r, g, b);
			glVertex2f(x, y);
			glVertex2f(x + side_x, y);
			glVertex2f(x + side_x, y + side_y);
			glVertex2f(x, y + side_y);
		glEnd();
	}, [&]{
		display.swapBuffers();
	}, [&]{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, display.width, 0, display.height, 1, -1); 
		glMatrixMode(GL_MODELVIEW);

		display.focus();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});

	// auto th = std::thread([&](){
	// 	// better spawn thread that will create interrupt
	// 	// when key is ready

		// get key if exists
	// 	while (display.active)
	// 		if(display.handleInput())
	// 			if (display.keyboard.getKeyState(display.keyboard.ESC))
	// 				display.requestClose();
	// 		// else if (!display.keyboard.queEmpty())
	// 		// 	spawn interrupt display.keyboard.popEvent();
	// });

	mobo.insertCpu(cpu);
	mobo.insertRam(ram);
	mobo.initVga(vga);

	while (display.active) {
		if(display.handleInput())
			if (display.keyboard.getKeyState(display.keyboard.ESC))
				display.requestClose();

		vga.vmem[0] = 0x0000'0200 + 0;
		vga.vmem[1] = 0x0000'0300 + 1;
		vga.vmem[2] = 0x0000'0400 + 2;
		vga.vmem[3] = 0x0000'0500 + 0;
		vga.vmem[4] = 0x0000'0600 + 1;
		vga.vmem[5] = 0x0000'0700 + 2;
		vga.vmem[6] = 0x0000'0800 + 0;
		vga.vmem[86] = 0x0000'0900 + 1;
		vga.focus();
		vga.display_text();
		vga.swapBuffers();
		mobo.update();
	}

	// if (th.joinable())
	// 	th.join();

	// while () {
	// 	// right now the window will exit at esc
	// 	if (display.handleInput()) {
	// 		if (display.keyboard.getKeyState(display.keyboard.ESC))
	// 			display.requestClose();
	// 		else {
	// 			while (!display.keyboard.queEmpty()) {
	// 				// keyboard driver could stay here
	// 				display.keyboard.popEvent(); // the key
	// 			}
	// 		}
	// 	}
	// 	display.focus();

	// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 	glBegin(GL_QUADS);
	// 		glColor3f(0.5, 0.5, 0.5);
	// 		glVertex2i(10, 100);
	// 		glVertex2i(100, 200);
	// 		glVertex2i(300, 10);			
	// 		glVertex2i(10, 10);
	// 	glEnd();
		
	// 	display.swapBuffers();
	// }
	return 0;
}
