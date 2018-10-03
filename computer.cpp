#include <iostream>
#include <verilated.h>
#include "obj_dir/Vcpu.h"
#include "OpenglWindow.h"

int main(int argc, char const *argv[])
{
	OpenglWindow display(640, 480, "VGA Display");
	Vcpu cpu("cpu");

	while (display.active) {
		// right now the window will exit at close
		if (display.handleInput()) {
			if (display.keyboard.getKeyState(display.keyboard.ESC))
				display.requestClose();
			else {
				while (!display.keyboard.queEmpty()) {
					cpu.A = display.keyboard.popEvent().key;
					cpu.B = 0;
					cpu.eval();
					std::cout << "A: " << cpu.A << " B: " << cpu.B << std::endl;
				}
			}
		}
		display.focus();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
			glColor3f(0.5, 0.5, 0.5);
			glVertex2f(-0.5, 0.5);
			glVertex2f(0.5, 0.5);
			glVertex2f(0.5, -0.5);			
			glVertex2f(-0.5, -0.5);
		glEnd();
		
		display.swapBuffers();
	}
	return 0;
}
