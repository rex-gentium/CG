#pragma once
#include "DrawObject2D.h"
class Point2D :
	public DrawObject2D
{
public:
	Point2D(Window * window, ScreenCoord position);
	~Point2D() override;

	void draw() override;
	std::vector<GLfloat> calculateVertices() override;
private:
	ScreenCoord position; 
	float size = 2.0f;
};

