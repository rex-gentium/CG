#include "DrawObject2D.h"
#include <glm\gtx\rotate_vector.hpp>


DrawObject2D::DrawObject2D(Window * window)
{
	this->window = window;
}

DrawObject2D::~DrawObject2D()
{
	if (vertexCount <= 1)
		delete vertices;
	else delete[] vertices;
}

void DrawObject2D::rotate(float angle)
{
	rotation += angle;
}

