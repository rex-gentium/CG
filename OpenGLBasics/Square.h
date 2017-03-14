#pragma once
#include "Rectangle.h"
class Square :
	public Rectangle
{
public:
	Square(Window * window, ScreenPoint coordinate, int width) : Rectangle(window, coordinate, width, width) {};
	Square(Window * window, RelativePoint coordinate, float width) : Rectangle(window, coordinate, width, width) {};
};

