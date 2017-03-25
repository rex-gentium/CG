#include "DrawObject2D.h"
#include <glm\gtx\rotate_vector.hpp>


DrawObject2D::DrawObject2D(Window * window)
{
	this->window = window;
}

DrawObject2D::~DrawObject2D()
{
}

void DrawObject2D::rotate(float angle)
{
	rotation += angle;
	float twoPi = glm::two_pi<float>();
	while (rotation > twoPi)
		rotation -= twoPi;
	while (rotation < -twoPi)
		rotation += twoPi;
}

void DrawObject2D::translate(glm::vec2 direction, float distance)
{
	position.x += glm::floor(direction.x * distance);
	position.y += glm::floor(direction.y * distance);
	if (position.x > window->getScreenWidth())
		position.x = window->getScreenWidth();
	if (position.y > window->getScreenHeight())
		position.y = window->getScreenHeight();
}

