#include "Point2D.h"
#include <algorithm>
#include <glm/gtx/rotate_vector.hpp>

Point2D::Point2D(Window * window, ScreenCoord position):
	DrawObject2D(window),
	position(position)
{
}

Point2D::~Point2D()
{
}

void Point2D::draw()
{
	glPointSize(size);
	auto vertices = calculateVertices();
	GLfloat * gVertexBufferData = vertices.data();
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), gVertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_TRUE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableVertexAttribArray(0);
}

std::vector<GLfloat> Point2D::calculateVertices()
{
	std::vector<GLfloat> result(2);
	result[0] = window->screenXToNormalizedX(position.x);
	result[1] = window->screenYToNormalizedY(position.y);
	return result;
}
