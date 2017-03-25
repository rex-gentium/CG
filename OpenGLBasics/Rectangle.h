#pragma once
#include "DrawObject2D.h"

class Rectangle:
	public DrawObject2D
{
public:
	Rectangle(Window * window, ScreenCoord coordinate, int width, int height);
	virtual ~Rectangle();

	void draw() override;
	std::vector<GLfloat> calculateVertices() override;
private:
	int width, height;
};

