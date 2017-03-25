#pragma once
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include "Types.h"
#include <glm/glm.hpp>

class Window
{
public:
	Window();
	~Window();

	int create(std::string windowName, int screenWidth, int screenHeight);
	void swapBuffers();

	int getScreenWidth() const { return screenWidth; }
	int getScreenHeight() const { return screenHeight; }
	float getScreenRatio() const { return screenWidth / (float)screenHeight; }
	float getScreenWidthChange() const { return screenWidth / (float) prevWidth; }
	float getScreenHeightChange() const { return screenHeight / (float)prevHeight; }

	GLfloat screenXToNormalizedX(int horizontalCoordinate) const;
	GLfloat screenYToNormalizedY(int verticalCoordinate) const;
	int normalizedXToScreenX(GLfloat horizontalCoordinate) const;
	int normalizedYToScreenY(GLfloat verticalCoordinate) const;
	ScreenCoord normalizedToScreen(NormalizedCoord coordinate) const;
	NormalizedCoord screenToNormalized(ScreenCoord coordinate) const;

	bool shouldClose() const;

	void clearDrawingBuffer();
	void drawBackgroundColor(const ColorRGBA);

	GLFWwindow* get() { return glfwWindow; }
private:
	GLFWwindow* glfwWindow;
	int screenWidth, screenHeight, prevWidth, prevHeight;
	
	void updateScreenResolution();
};

