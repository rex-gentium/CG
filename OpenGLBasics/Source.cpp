#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Window.h"
#include "common\shader.hpp"
#include "TextureLoader.h"

static const GLfloat g_vertex_buffer_data[] = {
	// низ: z = -1
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f,-1.0f,
	// верх: z = 1
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	// тыл: y = -1
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	// фронт: y = 1
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	// левый бок: х = -1
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	// правый бок: x = 1
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f
};

static const GLfloat g_uv_buffer_data[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f
};

static const GLfloat g_uv_buffer_data_repeat[] = {
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 4.0f,
	4.0f, 4.0f,
	0.0f, 0.0f,
	4.0f, 4.0f,
	4.0f, 0.0f
};
void initGLFW();
void initGLEW();

void initInputListeners(GLFWwindow * wnd);
void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void handleMouseMovement(GLFWwindow* window, double xpos, double ypos);
void handleMouseInput(GLFWwindow* window, int button, int action, int mods);
void computeMatricesFromInputs(Window * window);

GLuint vertexArrayObject;

bool isCameraMovingLeft, isCameraMovingRight, isCameraMovingForward, isCameraMovingBackward;
glm::mat4 view;
glm::mat4 projection;
glm::mat4 model = glm::mat4(1.0f); // model is in the center of the world
// Initial position : on +Z
glm::vec3 position = glm::vec3(3, 3, 3);
// горизонтальный угол: направление камеры от оси Y (радиан)
float horizontalAngle = 3.14f * 1.25f;
// вертикальный угол: отклонение камеры от плоскости XoY 
// (радиан, 0 - вдоль плоскости, pi/2 - перпендикулярно плоскости)
float verticalAngle = - 3.14f * 0.25f;

int main() {
	isCameraMovingLeft = isCameraMovingRight = isCameraMovingForward = isCameraMovingBackward = false;
	// init Window Object and input handlers
	initGLFW();
	Window window;
	bool isWindowCreated = window.create("Computer Graphics", 600, 600) == 0;;
	if (!isWindowCreated) return -1;
	initGLEW();
	initInputListeners(window.get());
	glfwSetCursorPos(window.get(), window.getScreenWidth() / 2.0f,
		window.getScreenHeight() / 2.0f);
	// Enable depth test & accept fragment if it closer to the camera than the former one
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// init Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// init Shaders
	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	
	GLuint matrixID = glGetUniformLocation(programID, "MVP");
	
	// loading texture and sampler
	GLuint texture = TextureLoader::loadDDS("uvtemplate.DDS");
	texture = TextureLoader::loadBMP_custom("uvtemplate.bmp");
	GLuint textureID = glGetUniformLocation(programID, "myTextureSampler");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// init and bind Vertex Buffer Object
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	// init and bind Color Buffer Object
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
	
	do {
		// draw code
		window.clearDrawingBuffer();
		window.drawBackgroundColor(ColorRGBA::BLUE);
		
		glUseProgram(programID);

		computeMatricesFromInputs(&window);
		glm::mat4 MVP = projection * view * model;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3*12);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		window.swapBuffers();
		glfwPollEvents();
	} 
	while (!window.shouldClose());

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteTextures(1, &textureID);

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
	if (key == GLFW_KEY_A)
		switch (action) {
		case GLFW_PRESS: isCameraMovingLeft = true; break;
		case GLFW_RELEASE: isCameraMovingLeft = false; break;
		}
	if (key == GLFW_KEY_D)
		switch (action) {
		case GLFW_PRESS: isCameraMovingRight = true; break;
		case GLFW_RELEASE: isCameraMovingRight = false; break;
		}
	if (key == GLFW_KEY_W)
		switch (action) {
		case GLFW_PRESS: isCameraMovingForward = true; break;
		case GLFW_RELEASE: isCameraMovingForward = false; break;
		}
	if (key == GLFW_KEY_S)
		switch (action) {
		case GLFW_PRESS: isCameraMovingBackward = true; break;
		case GLFW_RELEASE: isCameraMovingBackward = false; break;
		}
}

void handleMouseMovement(GLFWwindow * window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}

void handleMouseInput(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
		//isRotationMode = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
		//isRotationMode = false;
}

void computeMatricesFromInputs(Window * window)
{
	// Get mouse position
	static double xpos, ypos;
	glfwGetCursorPos(window->get(), &xpos, &ypos);
	// Reset mouse position for next frame
	glfwSetCursorPos(window->get(), window->getScreenWidth() / 2.0f, 
		window->getScreenHeight() / 2.0f);
	// Compute new orientation
	horizontalAngle += 0.001f * float(window->getScreenWidth() / 2.0f - xpos);
	verticalAngle += 0.001f * float(window->getScreenHeight() / 2.0f - ypos);
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	// Moving camepa position
	if (isCameraMovingForward)
		position += 0.02f * direction;
	if (isCameraMovingBackward)
		position -= 0.02f *direction;
	if (isCameraMovingRight)
		position += 0.02f * right;
	if (isCameraMovingLeft)
		position -= 0.02f * right;

	projection = glm::perspective(45.0f, window->getScreenRatio(), 0.1f, 100.0f);
	view = glm::lookAt(position, position + direction, up);
}
