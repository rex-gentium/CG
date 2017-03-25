#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Window.h"
#include "Point2D.h"
#include "Square.h"

int main();

void initGLFW();
void initGLEW();
void initVAO();
void initVBO();
void disposeVAO();
void disposeVBO();

void initInputListeners(GLFWwindow * wnd);
void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void handleMouseMovement(GLFWwindow* window, double xpos, double ypos);
void handleMouseInput(GLFWwindow* window, int button, int action, int mods);

GLuint vertexArrayObject;
GLuint vertexBufferObject;

bool isRotationMode = false, isTranslationMode = false;

int main() {
	initGLFW();
	Window window;
	bool isWindowCreated = window.create("Computer Graphics", 800, 600) == 0;
	if (!isWindowCreated) return -1;
	initGLEW();

	initVAO();
	initVBO();
	initInputListeners(window.get());
	
	Square square(&window, ScreenCoord(100, 100), 200);

	do {
		// draw code
		window.clearDrawingBuffer();
		window.drawBackgroundColor(ColorRGBA::WHITE);
		if (isRotationMode)
			square.rotate(0.03f);
		if (isTranslationMode)
			square.translate(glm::normalize(glm::vec2(1.0f, 0.0f)), 1.0f);
		square.draw();
		window.swapBuffers();
		glfwPollEvents();
	} 
	while (!window.shouldClose());

	disposeVAO();
	disposeVBO();

	return 0;
}

void initGLFW() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(-1);
	}
}

void initGLEW() {
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(-1);
	}
}

void initVAO() {
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
}

void initVBO() {
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
}

void disposeVAO() {
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void disposeVBO() {
	glDeleteBuffers(1, &vertexBufferObject);
}

void initInputListeners(GLFWwindow * wnd)
{
	glfwSetInputMode(wnd, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed
	glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(wnd, handleKeyInput);
	glfwSetCursorPosCallback(wnd, handleMouseMovement);
	glfwSetMouseButtonCallback(wnd, handleMouseInput);
}

void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_SPACE)
		switch (action) {
		case GLFW_PRESS: isTranslationMode = true; break;
		case GLFW_RELEASE: isTranslationMode = false; break;
		}
		
}

void handleMouseMovement(GLFWwindow * window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}

void handleMouseInput(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		isRotationMode = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		isRotationMode = false;
}
