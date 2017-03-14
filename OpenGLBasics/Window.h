#pragma once
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include "Types.h"

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

	GLfloat wndXToRelative(unsigned horizontalCoordinate) const;
	GLfloat wndYToRelative(unsigned verticalCoordinate) const;
	unsigned relativeXToWndCoordinate(GLfloat horizontalCoordinate) const;
	unsigned relativeYToWndCoordinate(GLfloat verticalCoordinate) const;
	ScreenPoint relativeToScreen(RelativePoint coordinate) const;
	RelativePoint screenToRelative(ScreenPoint coordinate) const;

	bool shouldClose() const;

	void clearDrawingBuffer();
	void drawBackgroundColor(const ColorRGBA);

	GLFWwindow* get() { return glfwWindow; }
private:
	GLFWwindow* glfwWindow;
	int screenWidth, screenHeight, prevWidth, prevHeight;
	
	void updateScreenResolution();
};

