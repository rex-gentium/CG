#pragma once
#include "DrawObject2D.h"
class Point2D :
	public DrawObject2D
{
public:
	Point2D(Window * window, ScreenPoint coordinate);
	Point2D(Window * window, RelativePoint coordinate);
	~Point2D() override;

	void draw() override;
	void updateVertices() override;
private:
	ScreenPoint point; 
	RelativePoint pointRelative;
	float size = 2.0f;
};

