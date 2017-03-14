#pragma once
#include "Window.h"
#include <glm\glm.hpp>
#include <vector>

class DrawObject2D
{
public:
	bool lockPosition = false, lockProportion = false, lockSize = false;

	DrawObject2D(Window * window);
	virtual ~DrawObject2D();

	virtual void draw() = 0;
	void rotate(float angle);
	
protected:
	Window * window;
	int vertexCount;
	glm::vec2 * vertices;
	virtual void updateVertices() = 0;
	float rotation = 0.0f;
};

