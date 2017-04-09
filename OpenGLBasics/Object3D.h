#pragma once
#include "Window.h"
#include <glm\glm.hpp>
#include <vector>

class Object3D
{
public:
	Object3D(Window * window);
	virtual ~Object3D();

	virtual void draw() = 0;
	//void rotate(float angle);
	void translate(glm::vec3 direction, float distance);

protected:
	Window * window;
	virtual std::vector<GLfloat> calculateVertices() = 0;
	glm::vec3 position;
	//float rotation = 0.0f;
};

