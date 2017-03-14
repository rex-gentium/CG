#pragma once

struct ScreenPoint {
	ScreenPoint() {};
	ScreenPoint(unsigned horizontal, unsigned vertical): 
		x(horizontal), y(vertical) {}
	unsigned x, y;
};

struct RelativePoint {
	RelativePoint() {};
	RelativePoint(float horizontal, float vertical) :
		x(horizontal), y(vertical) {}
	float x, y;
};

struct ColorRGBA {
	float red, green, blue, alpha;
	static const ColorRGBA BLACK, WHITE, RED, GREEN, BLUE;
};

