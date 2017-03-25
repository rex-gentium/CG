#pragma once
#include "Rectangle.h"
class Square :
	public Rectangle
{
public:
	Square(Window * window, ScreenCoord coordinate, int width) : Rectangle(window, coordinate, width, width) {};
};

