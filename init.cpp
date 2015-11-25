#ifndef _INIT_H_
#define _INIT_H_

#include <GL/gl3w.h>
#include <GL/glfw.h>
#include "GL/GL.h"

static const unsigned screenSizeX = 200, screenSizeY = 200;

struct Init {
	Init() {
		if(!glfwInit()) {
			std::cerr << "\nCould not initialize glfw!";
			return;
		}

		glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, true);

		if(!glfwOpenWindow(screenSizeX, screenSizeY, 8, 8, 8, 8, 8, 8, GLFW_WINDOW)) {
			std::cerr << "\nCould not open the window!";
			glfwTerminate();
			return;
		}

		glfwSetWindowTitle("Colorful Game of Life");
		glfwSetWindowPos(350, 150);
		glfwSwapInterval(0);
		glfwSetTime(0);

		if(gl3wInit() != 0) {
			std::cerr << "\nCould not initialize gl3w!";
			glfwTerminate();
			return;
		}
	}

	~Init() {
		glfwTerminate();
	}
};

#endif //_INIT_H_
