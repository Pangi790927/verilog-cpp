#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

/*
	Options:
			- options are per window
		vSync -> t/f, sets if vSync is enabled

	WindowType Functions:
		requestClose();		// ask the window to close
		setVSync();			// sets vsinc using option
		resize();			// resizes viewPort
		focus();			// ready window for drawing
		swapBuffers();		// swap the drawing buffers
		handleInput();		// remembers input pressed,
							// returns true if event occured
		toString()			// returns a string that describes the window 
*/
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Options.h"
#if defined(__linux__)
	#include "LinuxWindow.h"
	using RawWindow = LinuxWindow;
#elif defined(_WIN32)
	#include "WindowsWindow.h"
	std::map<HWND, WindowsWindow *> WindowsWindow::eventMap;
	using RawWindow = WindowsWindow;
#endif

class OpenglWindow : public RawWindow {
public:
	Options options;

	OpenglWindow (int width, int height, std::string name = "name",
			int msaa = 8, decltype(RawWindow::window) parrent = 0,
			Options options = Options())
	: RawWindow(width, height, name, msaa, parrent), options(options)
	{
		setVSync(options["vSync"]);
		initGlew();
	}

	void initGlew() {
		GLenum err = glewInit();
		if (err != GLEW_OK)
			throw std::runtime_error(std::string("glew error: ") +
					(char *)glewGetErrorString(err));
	}
};

#endif