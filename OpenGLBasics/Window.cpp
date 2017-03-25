#include "Window.h"

Window::Window()
{
}


Window::~Window()
{
}

int Window::create(std::string windowName, int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	this->glfwWindow = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), NULL, NULL);
	if (glfwWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 4.5 compatible.\n");
		glfwTerminate();
		glfwWindow = nullptr;
		return -1;
	}
	
	glfwMakeContextCurrent(glfwWindow);
	
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f);

	return 0;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
	//updateScreenResolution();
}

GLfloat Window::screenXToNormalizedX(int horizontalCoordinate) const
{
	return GLfloat(2.0f * horizontalCoordinate / screenWidth - 1.0f);
}

GLfloat Window::screenYToNormalizedY(int verticalCoordinate) const
{
	return GLfloat(1.0f - 2.0f * verticalCoordinate / screenHeight);
}

int Window::normalizedXToScreenX(GLfloat horizontalCoordinate) const
{
	return static_cast<unsigned>((horizontalCoordinate + 1.0f) * screenWidth / 2.0f);
}

int Window::normalizedYToScreenY(GLfloat verticalCoordinate) const
{
	return static_cast<unsigned>((1.0f - verticalCoordinate) * screenHeight / 2.0f);
}

ScreenCoord Window::normalizedToScreen(NormalizedCoord coordinate) const
{
	return ScreenCoord({ normalizedXToScreenX(coordinate.x), normalizedYToScreenY(coordinate.y) });
}

NormalizedCoord Window::screenToNormalized(ScreenCoord coordinate) const
{
	return NormalizedCoord({ screenXToNormalizedX(coordinate.x), screenYToNormalizedY(coordinate.y) });
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(glfwWindow) != 0; // Alt+F4, or exit button clicked
}

void Window::clearDrawingBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateScreenResolution();
}

void Window::drawBackgroundColor(const ColorRGBA color)
{
	GLfloat rgba[] = {color.red, color.green, color.blue, color.alpha};
	glClearBufferfv(GL_COLOR, 0, rgba);
}

void Window::updateScreenResolution()
{
	prevWidth = screenWidth;
	prevHeight = screenHeight;
	glfwGetFramebufferSize(glfwWindow, &screenWidth, &screenHeight);
	if (prevWidth != screenWidth || prevHeight != screenHeight) {
		glViewport(0, 0, screenWidth, screenHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
	}
}

