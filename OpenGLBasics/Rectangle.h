#pragma once
#include "DrawObject2D.h"

class Rectangle:
	public DrawObject2D
{
public:
	Rectangle(Window * window, ScreenPoint coordinate, int width, int height);
	Rectangle(Window * window, RelativePoint coordinate, float width, float height);
	virtual ~Rectangle();

	void draw() override;
	void updateVertices() override;
private:
	ScreenPoint point;
	RelativePoint pointRelative;
	int width, height;
	float widthRelative, heightRelative;
};

