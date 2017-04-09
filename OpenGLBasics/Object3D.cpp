#include "Object3D.h"



Object3D::Object3D(Window * window)
{
	this->window = window;
}

Object3D::~Object3D()
{
}

void Object3D::translate(glm::vec3 direction, float distance)
{
	direction = glm::normalize(direction);
	position += direction * distance;
}
