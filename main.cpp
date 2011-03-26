#include <iostream>
#include <boost/foreach.hpp>
#include "GL/Program.h"

#define foreach BOOST_FOREACH

#include "init.cpp"
static const Init _;

static const unsigned textureSizeX = screenSizeX;
static const unsigned textureSizeY = screenSizeY;
static const unsigned RGBA = 4;
static const unsigned size = textureSizeX * textureSizeY * RGBA;

static unsigned vboVertices, vboIndices, tbo, textureIndex;
static Program program("Shaders/vertex.glsl", "Shaders/fragment.glsl");

static float * rawPixels = new float[size];
static float * rawPixelsHelper = new float[size];
static enum { RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3 } colouringMode = RED;

void setUp() {
	program.use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float vertices[] = {
			-1.0, -1.0, 0.0,
			-1.0, 1.0, 0.0,
			1.0, 1.0, 0.0,
			1.0, -1.0, 0.0
	};

	unsigned indices[] = {
			0, 1, 2,
			2, 3, 0
	};

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glGenBuffers(1, &vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned), indices, GL_STATIC_DRAW);

	glGenTextures(1, &textureIndex);
	glBindTexture(GL_TEXTURE_BUFFER, textureIndex);
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_TEXTURE_BUFFER, tbo);
	glBufferData(GL_TEXTURE_BUFFER, size * sizeof(float), 0, GL_DYNAMIC_DRAW);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tbo);

	glUniform1i(program.uniform("sampler"), 0);	//Texture Unit 0
	glUniform1i(program.uniform("screenSizeX"), screenSizeX);
	glUniform1i(program.uniform("screenSizeY"), screenSizeY);

	for(unsigned x = 0; x < textureSizeX; ++x) {
		for(unsigned y = 0; y < textureSizeY; ++y) {
			unsigned i = (y * textureSizeX + x) * RGBA;
			rawPixels[i] = 0.0;
			rawPixels[i+1] = 0.0;
			rawPixels[i+2] = 0.0;
			rawPixels[i+3] = 1.0;
		}
	}
	for(unsigned i = 0; i < size; ++ i) {
		rawPixelsHelper[i] = rawPixels[i];
	}
}

inline void draw() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void cleanUp() {
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	program.dontUse();

	glBindTexture(GL_TEXTURE_BUFFER, 0);
	if(glIsTexture(textureIndex)) glDeleteTextures(1, &textureIndex);	//TODO doesn't work - GL memory leak
	if(glIsBuffer(tbo)) glDeleteBuffers(1, &tbo);
	if(glIsBuffer(vboVertices)) glDeleteBuffers(1, &vboVertices);
	if(glIsBuffer(vboIndices)) glDeleteBuffers(1, &vboIndices);

	delete[] rawPixels;
	delete[] rawPixelsHelper;

	checkgl;
}

inline void writeData(float * data) {
	glBufferData(GL_TEXTURE_BUFFER, size * sizeof(float), data, GL_DYNAMIC_DRAW);
}

void GLFWCALL mousePositionCallback(int x, int y) {
	if(x >= 0 && x < static_cast<int>(screenSizeX) && y >= 0 && y < static_cast<int>(screenSizeY)) {
		y = (screenSizeY - 1) - y;
		x *= static_cast<float>(textureSizeX) / screenSizeX;
		y *= static_cast<float>(textureSizeY) / screenSizeY;
		unsigned i = ( y * (textureSizeX - 1) + x ) * RGBA;
		rawPixels[i + colouringMode] = (colouringMode != ALPHA) ? 1.0 : 0.0;
	}
}

void GLFWCALL mouseButtonCallback(int button, int action) {
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		if(action == GLFW_PRESS) {
			glfwSetMousePosCallback(mousePositionCallback);
		} else {
			glfwSetMousePosCallback(0);
		}
	}
}

void GLFWCALL keyCallback(int key, int action) {
	if(action == GLFW_PRESS) {
		switch(key) {
		case '1':
			colouringMode = RED;
			break;
		case '2':
			colouringMode = GREEN;
			break;
		case '3':
			colouringMode = BLUE;
			break;
		case '4':
			colouringMode = ALPHA;
			break;
		default:
			break;
		}
	}
}

void colour() {
	glfwSetKeyCallback(keyCallback);
	glfwSetMouseButtonCallback(mouseButtonCallback);
	while(!glfwGetKey(GLFW_KEY_SPACE)) {
		if(!glfwGetWindowParam(GLFW_OPENED) || glfwGetKey(GLFW_KEY_ESC)) break;
		if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
			glClear(GL_COLOR_BUFFER_BIT);
			writeData(rawPixels);
		}
		draw();
		glfwSwapBuffers();
	}
	glfwSetMouseButtonCallback(0);
	glfwSetKeyCallback(0);
}

inline unsigned getNumberOfNeighbours(unsigned i) {
	unsigned n = 0;
	if(rawPixels[i + RGBA] == 1.0) ++n;
	if(rawPixels[i - RGBA] == 1.0) ++n;
	if(rawPixels[i + (textureSizeX - 1) * RGBA] == 1.0) ++n;
	if(rawPixels[i + (textureSizeX - 1) * RGBA + RGBA] == 1.0) ++n;
	if(rawPixels[i + (textureSizeX - 1) * RGBA - RGBA] == 1.0) ++n;
	if(rawPixels[i - (textureSizeX - 1) * RGBA] == 1.0) ++n;
	if(rawPixels[i - (textureSizeX - 1) * RGBA + RGBA] == 1.0) ++n;
	if(rawPixels[i - (textureSizeX - 1) * RGBA - RGBA] == 1.0) ++n;
	return n;
}

inline void animate(unsigned i, bool animateTheDead = false) {
	float dead = animateTheDead ? 1.0 : 0.0;
	float alive = animateTheDead ? 0.0 : 1.0;
	unsigned n = getNumberOfNeighbours(i);
	if(rawPixels[i] == dead) {
		if(n == 3) rawPixelsHelper[i] = alive;
	} else {
		if(n <= 1 || n >= 4) rawPixelsHelper[i] = dead;
	}
}

void live() {
	for(unsigned x = 1; x < (textureSizeX - 1); ++x) {
		for(unsigned y = 1; y < (textureSizeY - 1); ++y) {
			unsigned i = (y * textureSizeX + x) * RGBA;

			animate(i);
			animate(i + 1);
			animate(i + 2);
			animate(i + 3, true);
		}
	}
	for(unsigned i = 0; i < size; ++i) {
		rawPixels[i] = rawPixelsHelper[i];
	}
}

int main() {
	setUp();

	writeData(rawPixels);
	draw();
	colour();

	while(true) {
		if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) {
			break;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		live();
		writeData(rawPixels);
		draw();

		glfwSwapBuffers();
	}

	cleanUp();
	return 0;
}
