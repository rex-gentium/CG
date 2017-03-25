#pragma once
#include "Window.h"
#include <glm\glm.hpp>
#include <vector>

class DrawObject2D
{
public:
	//bool lockPosition = false, lockProportion = false, lockSize = false;

	DrawObject2D(Window * window);
	virtual ~DrawObject2D();

	virtual void draw() = 0;
	void rotate(float angle);
	void translate(glm::vec2 direction, float distance);
	
protected:
	Window * window;
	virtual std::vector<GLfloat> calculateVertices() = 0;
	ScreenCoord position;
	float rotation = 0.0f;
};

