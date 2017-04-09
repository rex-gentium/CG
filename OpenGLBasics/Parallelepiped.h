#pragma once
#include "Object3D.h"

class Parallelepiped: 
	public Object3D
{
public:
	Parallelepiped(Window * window, glm::vec3 position, int length, int width, int height);
	virtual ~Parallelepiped();

	void draw() override;
protected:
	std::vector<GLfloat> calculateVertices() override;
	int length, width, height;
};

