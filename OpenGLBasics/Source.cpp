﻿#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Window.h"
#include "common\shader.hpp"
#include "TextureLoader.h"
#include <vector>

typedef glm::vec3 Point;

struct Triangle {
	Point a, b, c;
	glm::vec3 normal;
	Triangle() {}
	Triangle(Point a, Point b, Point c): a(a), b(b), c(c) {
		normal = glm::normalize(glm::cross(b - a, c - a));
	}
	std::vector<Triangle> segment() const {
		std::vector<Triangle> result(4);
		Point ab_middle = 0.5f * (a + b), bc_middle = 0.5f * (b + c), ca_middle = 0.5f * (c + a);
		result[0] = Triangle(a, ab_middle, ca_middle);
		result[1] = Triangle(b, bc_middle, ab_middle);
		result[2] = Triangle(c, ca_middle, bc_middle);
		result[3] = Triangle(ab_middle, bc_middle, ca_middle);
		return result;
	}
	void normalize(Point center, float radius) {
		a = center + glm::normalize(a - center);
		b = center + glm::normalize(b - center);
		c = center + glm::normalize(c - center);
	}
	bool contains(Point x) { return a == x || b == x || c == x; }
	glm::vec3 getNormal() { return normal; }
};
class Shape {
	std::vector<Triangle> polygons;
public:
	Shape(std::vector<Triangle> polygons): polygons(polygons) {}
	GLfloat * getVertexBufferData(int& vertexBufferSize) const {
		vertexBufferSize = polygons.size() * 3 * 3; // 3 points, 3 coordinates
		GLfloat * result = new GLfloat[vertexBufferSize];
		for (int i = 0; i < polygons.size(); ++i) {
			result[9 * i] = polygons[i].a.x; result[9 * i + 1] = polygons[i].a.y; result[9 * i + 2] = polygons[i].a.z;
			result[9 * i + 3] = polygons[i].b.x; result[9 * i + 4] = polygons[i].b.y; result[9 * i + 5] = polygons[i].b.z;
			result[9 * i + 6] = polygons[i].c.x; result[9 * i + 7] = polygons[i].c.y; result[9 * i + 8] = polygons[i].c.z;
		}
		return result;
	}
	GLfloat * getUVBufferData(int& uvBufferSize) const {
		uvBufferSize = polygons.size() * 3 * 2; // 3 points, 2 dimensions
		GLfloat * result = new GLfloat[uvBufferSize];
		for (int i = 0; i < polygons.size(); ++i) {
			result[6 * i] = 0.0f; result[6 * i + 1] = 0.0f;
			result[6 * i + 2] = 0.5f; result[6 * i + 3] = 1.0f;
			result[6 * i + 4] = 1.0f; result[6 * i + 5] = 0.0f;
		}
		return result;
	}
	Shape * segment() const {
		std::vector<Triangle> segmentedPolygons(polygons.size() * 4);
		for (int i = 0; i < polygons.size(); ++i) {
			auto segments = polygons[i].segment();
			segmentedPolygons[4 * i] = segments[0];
			segmentedPolygons[4 * i + 1] = segments[1];
			segmentedPolygons[4 * i + 2] = segments[2];
			segmentedPolygons[4 * i + 3] = segments[3];
		}
		Shape * result = new Shape(segmentedPolygons);
		return result;
	}
	int getVertexCount() const { return polygons.size() * 3; }
	void normalize(Point center, float radius) {
		for (int i = 0; i < polygons.size(); ++i)
			polygons[i].normalize(center, radius);
	}
	glm::vec3 getVertexNormal(Point vertex) {
		glm::vec3 result;
		for (int i = 0; i < polygons.size(); ++i)
			if (polygons[i].contains(vertex)) {
				result += polygons[i].getNormal();
				glm::normalize(result);
			}
		return result;
	}
	GLfloat * getNormalBufferData(int& normalBufferSize) {
		normalBufferSize = polygons.size() * 3 * 3; // 3 points, 3 coordinates
		GLfloat * result = new GLfloat[normalBufferSize];
		for (int i = 0; i < polygons.size(); ++i) {
			Point aNormal = getVertexNormal(polygons[i].a), bNormal = getVertexNormal(polygons[i].b), cNormal = getVertexNormal(polygons[i].c);
			result[9 * i] = aNormal.x; result[9 * i + 1] = aNormal.y; result[9 * i + 2] = aNormal.z;
			result[9 * i + 3] = bNormal.x; result[9 * i + 4] = bNormal.y; result[9 * i + 5] = bNormal.z;
			result[9 * i + 6] = cNormal.x; result[9 * i + 7] = cNormal.y; result[9 * i + 8] = cNormal.z;
		}
		return result;
	}
};

GLfloat g_uv_buffer_data[] = {
	0.0f, 0.5f,
	0.0f, 1.0f,
	0.33f, 0.5f,
	0.33f, 0.5f,
	0.0f, 1.0f,
	0.66f, 0.5f,
	0.66f, 0.5f,
	0.0f, 1.0f,
	1.0f, 0.5f,
	1.0f, 0.5f,
	0.0f, 1.0f,
	0.0f, 0.5f,
	0.0f, 0.5f,
	0.33f, 0.5f,
	0.0f, 0.0f,
	0.33f, 0.5f,
	0.66f, 0.5f,
	0.0f, 0.0f,
	0.66f, 0.5f,
	1.0f, 0.5f,
	0.0f, 0.0f,
	1.0f, 0.5f,
	0.0f, 0.5f,
	0.0f, 0.0f
};

// OCTAHEDRON
Point a(-1.0f, 1.0f, 0.0f), 
b(1.0f, 1.0f, 0.0f), 
c(1.0f, -1.0f, 0.0f), 
d(-1.0f, -1.0f, 0.0f), 
je(0.0f, 0.0f, 1.0f), 
f(0.0f, 0.0f, -1.0f);
std::vector<Triangle> octahedronPolygons = {
	Triangle(a, je, b),
	Triangle(b, je, c),
	Triangle(c, je, d),
	Triangle(d, je, a),
	Triangle(a, b, f),
	Triangle(b, c, f),
	Triangle(c, d, f),
	Triangle(d, a, f)
};
Shape * octahedron = new Shape(octahedronPolygons);

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
	Shape * sphere = octahedron;
	for (int i = 0; i < 0; ++i) {
		 Shape * segmentationIteration = sphere->segment();
		 delete sphere;
		 sphere = segmentationIteration;
	}
	sphere->normalize({0.0f, 0.0f, 0.0f}, 1.0f);
	int vertexBufferDataSize;
	GLfloat * vertexBufferData = sphere->getVertexBufferData(vertexBufferDataSize);
	int uvBufferDataSize;
	GLfloat * uvBufferData = sphere->getUVBufferData(uvBufferDataSize);
	int normalsBufferDataSize;
	GLfloat * normalsBufferData = sphere->getNormalBufferData(normalsBufferDataSize);

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
	GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	
	GLuint matrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	
	// loading texture and sampler
	GLuint texture = TextureLoader::loadDDS("uvtemplate.DDS");
	texture = TextureLoader::loadBMP_custom("earth.bmp");
	GLuint textureID = glGetUniformLocation(programID, "myTextureSampler");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// init and bind Vertex Buffer Object
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexBufferDataSize, vertexBufferData, GL_STATIC_DRAW);
	// init and bind Color Buffer Object
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normalsBufferDataSize, normalsBufferData, GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	
	do {
		// draw code
		window.clearDrawingBuffer();
		window.drawBackgroundColor(ColorRGBA::BLUE);
		
		glUseProgram(programID);

		computeMatricesFromInputs(&window);
		glm::mat4 MVP = projection * view * model;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		glm::vec3 lightPos = glm::vec3(0, 8, 0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, sphere->getVertexCount());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		window.swapBuffers();
		glfwPollEvents();
	} 
	while (!window.shouldClose());

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalbuffer);
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

GLfloat * segmentOctahedron(GLfloat * octahedron, int &vertexCount, int precision)
{
	GLfloat * result = new GLfloat[3 * vertexCount * 4];
	for (int f = 0; f < vertexCount / 9; ++f) {
		glm::vec3 v1(octahedron[9 * f], octahedron[9 * f + 1], octahedron[9 * f + 2]);
		glm::vec3 v2(octahedron[9 * f + 3], octahedron[9 * f + 4], octahedron[9 * f + 5]);
		glm::vec3 v3(octahedron[9 * f + 6], octahedron[9 * f + 7], octahedron[9 * f + 8]);
		glm::vec3 a = (v1 + v2) * 0.5f;
		glm::vec3 b = (v2 + v3) * 0.5f;
		glm::vec3 c = (v1 + v3) * 0.5f;
		result[9 * 4 * f + 0] = v1.x; result[9 * 4 * f + 1] = v1.y; result[9 * 4 * f + 2] = v1.z;
		result[9 * 4 * f + 3] = a.x; result[9 * 4 * f + 4] = a.y; result[9 * 4 * f + 5] = a.z;
		result[9 * 4 * f + 6] = c.x; result[9 * 4 * f + 7] = c.y; result[9 * 4 * f + 8] = c.z;
		result[9 * 4 * f + 9] = a.x; result[9 * 4 * f + 10] = a.y; result[9 * 4 * f + 11] = a.z;
		result[9 * 4 * f + 12] = v2.x; result[9 * 4 * f + 13] = v2.y; result[9 * 4 * f + 14] = v2.z;
		result[9 * 4 * f + 15] = b.x; result[9 * 4 * f + 16] = b.y; result[9 * 4 * f + 17] = b.z;
		result[9 * 4 * f + 18] = a.x; result[9 * 4 * f + 19] = a.y; result[9 * 4 * f + 20] = a.z;
		result[9 * 4 * f + 21] = b.x; result[9 * 4 * f + 22] = b.y; result[9 * 4 * f + 23] = b.z;
		result[9 * 4 * f + 24] = c.x; result[9 * 4 * f + 25] = c.y; result[9 * 4 * f + 26] = c.z;
		result[9 * 4 * f + 27] = c.x; result[9 * 4 * f + 28] = c.y; result[9 * 4 * f + 29] = c.z;
		result[9 * 4 * f + 30] = b.x; result[9 * 4 * f + 31] = b.y; result[9 * 4 * f + 32] = b.z;
		result[9 * 4 * f + 33] = v3.x; result[9 * 4 * f + 34] = v3.y; result[9 * 4 * f + 35] = v3.z;
	}
	vertexCount = 3 * 4 * vertexCount;
	return result;
}
