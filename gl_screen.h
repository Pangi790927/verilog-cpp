#ifndef GL_SCREEN
#define GL_SCREEN

struct GlScreen {
	std::unique_ptr<uint8_t[]> screen = nullptr;
	GLuint gl_screen = 0;

	size_t width = 0;
	size_t height = 0;
	bool updated = true;

	GlScreen() {}
	GlScreen (size_t width, size_t height) : width(width), height(height) {
		screen = std::unique_ptr<uint8_t[]>(new uint8_t[width * height * 3]);

		glGenTextures(1, &gl_screen);
		glBindTexture(GL_TEXTURE_2D, gl_screen);
	
		// GL_LINEAR, GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			screen.get()
		);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void put_pixel (uint color, int x, int y) {
		if (!screen)
			return ;
		uint8_t r = (((0x000000ff & color) >> 0 ) & 0xff);
		uint8_t g = (((0x0000ff00 & color) >> 8 ) & 0xff);
		uint8_t b = (((0x00ff0000 & color) >> 16) & 0xff);

		if (!((height - y) * width + x > width * height || y > height || x < 0)) {
			screen[((height - y) * width + x) * 3 + 0] = r;
			screen[((height - y) * width + x) * 3 + 1] = g;
			screen[((height - y) * width + x) * 3 + 2] = b;
		}

		updated = true;
	}

	void render() {
		if (!screen)
			return ;

		if (updated) {
			glBindTexture(GL_TEXTURE_2D, gl_screen);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					screen.get()
				);
			glBindTexture(GL_TEXTURE_2D, 0);
			updated = false;
		}
		glBindTexture(GL_TEXTURE_2D, gl_screen);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1);

			glTexCoord2f(1, 0);
			glVertex2f(1, -1);

			glTexCoord2f(1, 1);
			glVertex2f(1, 1);

			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

#endif

