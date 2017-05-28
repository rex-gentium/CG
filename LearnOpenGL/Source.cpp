#include <iostream>
#include <cmath>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Shape3D.h"
#include "Light.h"

// Properties
const GLuint WIDTH = 1024, HEIGHT = 768;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processMovement();

// Camera
Camera camera(glm::vec3(2.37282f, 1.06039f, 0.775822f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Project", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Build and compile our shader program
	//Shader lightingShader("GouraudVertexShader.vs", "GouraudFragmentShader.fs");
	Shader lightingShader("PhongVertexShader.vs", "PhongFragmentShader.fs");
	Shader lampShader("PhongVertexShader.vs", "WhiteFragmentShader.fs");

	// Light properties
	Light lightSource = Light::DEFAULT;
	lightSource.position = { 1.2f, 1.0f, 1.5f };

	// Set up vertex data (and buffer(s)) and attribute pointers
	Point a(0.0f, 0.5f, 0.0f), 
		b(0.5f, 0.0f, 0.0f), 
		c(0.0f, -0.5f, 0.0f), 
		d(-0.5f, 0.0f, 0.0f), 
		e(0.0f, 0.0f, 0.5f), 
		f(0.0f, 0.0f, -0.5f);
	Shape * octahedron = new Shape({
		Triangle(a, e, b),
		Triangle(b, e, c),
		Triangle(c, e, d),
		Triangle(d, e, a),
		Triangle(a, b, f),
		Triangle(b, c, f),
		Triangle(c, d, f),
		Triangle(d, a, f) }, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Shape * sphere = octahedron->segment(7);
	sphere->normalize({ 0.0f, 0.0f, 0.0f }, 0.5f);
	int vertexBufferDataSize;
	GLfloat * verticesBufferData = sphere->getVertexNormalBufferData(vertexBufferDataSize);

	Shape * glass = new Shape({
		Triangle({ 0.0f, -0.5f, -0.5f },{ 0.0f, 0.5f, -0.5f },{ 0.0f, 0.5f, 0.5f }),
		Triangle({ 0.0f, -0.5f, -0.5f },{ 0.0f, 0.5f, 0.5f },{ 0.0f, -0.5f, 0.5f })
	});
	int glassVertexBufferDataSize;
	GLfloat * glassVerticesBufferData = glass->getVertexNormalBufferData(glassVertexBufferDataSize);

	std::vector<Shape*> glasses;
	glasses.push_back(new Shape(glass->polygons, glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.5f)));
	glasses.push_back(new Shape(glass->polygons, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.5f)));
	glasses.push_back(new Shape(glass->polygons, glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, glm::radians(90.0f), 0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f)));

	// Sphere VAO
	GLuint sphereVAO, sphereVBO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexBufferDataSize, verticesBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); //position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // normal
	glBindVertexArray(0);
	// Lamp VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // position
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	// Glass VAO
	GLuint glassVAO, glassVBO;
	glGenVertexArrays(1, &glassVAO);
	glGenBuffers(1, &glassVBO);
	glBindVertexArray(glassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, glassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * glassVertexBufferDataSize, glassVerticesBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); //position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // normal
	glBindVertexArray(0);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		processMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.use();		
		GLint lightPosLoc = glGetUniformLocation(lightingShader.program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.program, "viewPos");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.program, "lightColor");
		GLint objColorLoc = glGetUniformLocation(lightingShader.program, "objectColor");
		GLint lightAmbientStrengthLoc = glGetUniformLocation(lightingShader.program, "ambientStrength");
		GLint lightDiffuseStrengthLoc = glGetUniformLocation(lightingShader.program, "diffuseStrength");
		GLint lightSpecularStrengthLoc = glGetUniformLocation(lightingShader.program, "specularStrength");
		glUniform3f(lightPosLoc, lightSource.position.x, lightSource.position.y, lightSource.position.z);
		glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform1f(lightAmbientStrengthLoc, 0.2f);
		glUniform1f(lightDiffuseStrengthLoc, 0.7f);
		glUniform1f(lightSpecularStrengthLoc, 1.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(camera.zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		
		std::map<float, Shape*> sortedGlasses;
		for (auto & glass : glasses) {
			GLfloat distance = glm::length(camera.position - glass->position);
			sortedGlasses[distance] = glass;
		}

		// Draw Sphere
		glUniform4f(objColorLoc, sphere->color.r, sphere->color.g, sphere->color.b, sphere->color.a);
		sphere->draw(sphereVAO, lightingShader, view, projection);
		// Draw Glasses
		for (auto it = sortedGlasses.rbegin(); it != sortedGlasses.rend(); ++it)
		{
			Shape * glass = it->second;
			glUniform4f(objColorLoc, glass->color.r, glass->color.g, glass->color.b, glass->color.a);
			glass->draw(glassVAO, lightingShader, view, projection);
		}

		// Draw Lamp
		lampShader.use();
		GLint modelLoc = glGetUniformLocation(lampShader.program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model = glm::translate(glm::mat4(), lightSource.position);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, sphere->getVertexCount());
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void processMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
	std::cout << "(" << camera.position.x << " " << camera.position.y << " " << camera.position.z << ")" << std::endl;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

