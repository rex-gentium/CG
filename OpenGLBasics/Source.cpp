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
void handleInput(GLFWwindow * wnd, Square square);

GLuint vertexArrayObject;
GLuint vertexBufferObject;

int main() {
	initGLFW();
	Window window;
	bool isWindowCreated = window.create("Computer Graphics", 1024, 768) == 0;
	if (!isWindowCreated) return -1;
	initGLEW();

	initVAO();
	initVBO();
	initInputListeners(window.get());
	
	Square square(&window, ScreenPoint(200, 200), 100);
	square.lockProportion = true;

	do {
		//handleInput(window.get(), square);
		// draw code
		window.clearDrawingBuffer();
		window.drawBackgroundColor(ColorRGBA::WHITE);
		square.rotate(0.01f);
		square.draw();
		window.swapBuffers();
	} 
	while (!window.shouldClose() 
		|| glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS);

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
	//glfwSetInputMode(wnd, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed
	//glfwSetKeyCallback(window, key_callback);
}

void handleInput(GLFWwindow * wnd, Square square)
{
	int state = glfwGetKey(wnd, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
		square.rotate(-1.0f);
	state = glfwGetKey(wnd, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
		square.rotate(+1.0f);
}

/*
void drawTriangle(glm::uvec2 pos1, glm::uvec2 pos2, glm::uvec2 pos3) {
	// An array of 3 vectors which represents 3 vertices
	GLfloat gVertexBufferData[] = {
		wndXToRelative(pos1.x), wndYToRelative(pos1.y), 0.0f,
		wndXToRelative(pos2.x), wndYToRelative(pos2.y), 0.0f,
		wndXToRelative(pos3.x), wndYToRelative(pos3.y), 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}

void drawTriangleRelative(glm::f32vec2 pos1, glm::f32vec2 pos2, glm::f32vec2 pos3) {
	// An array of 3 vectors which represents 3 vertices
	GLfloat gVertexBufferData[] = {
		pos1.x, pos1.y, 0.0f,
		pos2.x, pos2.y, 0.0f,
		pos3.x, pos3.y, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}

void drawRectangle(glm::uvec2 pos, int width, int height) {
	drawTriangle(pos, glm::uvec2(pos.x + width, pos.y), glm::uvec2(pos.x + width, pos.y + height));
	drawTriangle(pos, glm::uvec2(pos.x, pos.y + height), glm::uvec2(pos.x + width, pos.y + height));
}

void drawRectangleRelative(glm::f32vec2 pos, float width, float height) {
	drawTriangleRelative(glm::f32vec2(pos.x - width / 2, pos.y + height / 2), 
		glm::f32vec2(pos.x + width / 2, pos.y + height / 2), 
		glm::f32vec2(pos.x + width / 2, pos.y - height / 2));
	drawTriangleRelative(glm::f32vec2(pos.x + width / 2, pos.y - height / 2),
		glm::f32vec2(pos.x - width / 2, pos.y - height / 2),
		glm::f32vec2(pos.x - width / 2, pos.y + height / 2));
}

void drawSquare(glm::uvec2 pos, int width) {
	return drawRectangle(pos, width, width);
}

void drawSquareRelative(glm::f32vec2 pos, float width) {
	float k = (float)wndWidth / (float)wndHeight;
	drawRectangleRelative(pos, width, width * k);
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s\n", vertex_file_path);
		getchar();
		return 0;
	}
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return ProgramID;
}*/
