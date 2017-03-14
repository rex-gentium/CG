#include "Point2D.h"
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

Point2D::Point2D(Window * window, ScreenPoint coordinate):
	DrawObject2D(window),
	point(coordinate)
{
	pointRelative = window->screenToRelative(point);
	vertices = new glm::vec2(pointRelative.x, pointRelative.y);
}

Point2D::Point2D(Window * window, RelativePoint coordinate) : 
	DrawObject2D(window),
	pointRelative(coordinate)
{
	point = this->window->relativeToScreen(coordinate);
	vertices = new glm::vec2(pointRelative.x, pointRelative.y);
}

Point2D::~Point2D()
{
}

void Point2D::draw()
{
	if (lockPosition)
		updateVertices();
	if (!lockProportion)
		size *= window->getScreenWidthChange();

	glPointSize(size);
	GLfloat gVertexBufferData[] = { vertices->x, vertices->y, 0.0f };
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableVertexAttribArray(0);
}

void Point2D::updateVertices()
{
	*vertices = { window->wndXToRelative(point.x), window->wndYToRelative(point.y)};
	*vertices = glm::rotate(*vertices, rotation);
}
