#pragma once

struct ScreenCoord {
	ScreenCoord() {};
	ScreenCoord(int horizontal, int vertical):
		x(horizontal), y(vertical) {}
	int x, y;
};

struct NormalizedCoord {
	NormalizedCoord() {};
	NormalizedCoord(float horizontal, float vertical) :
		x(horizontal), y(vertical) {}
	float x, y;
};

struct ColorRGBA {
	float red, green, blue, alpha;
	static const ColorRGBA BLACK, WHITE, RED, GREEN, BLUE;
};

