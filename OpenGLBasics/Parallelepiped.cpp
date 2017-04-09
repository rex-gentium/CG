#include "Parallelepiped.h"

Parallelepiped::Parallelepiped(Window * window, glm::vec3 position, int length, int width, int height): Object3D(window)
{
	this->position = position;
	this->length = length;
	this->width = width;
	this->height = height;
}

Parallelepiped::~Parallelepiped()
{
}

void Parallelepiped::draw()
{
	auto vertices = calculateVertices();
	// An array of 36 vectors which represents 12 triangles
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
}

std::vector<GLfloat> Parallelepiped::calculateVertices()
{
	// в результирующий массив проталкивается 36 точек (12 треугольников) по 3 координаты
	glm::vec3 a = { 0.0f, 0.0f, 0.5f },
		b = a + glm::vec3(0.1f, 0.0f, 0.0f),
		c = a + glm::vec3(0.1f, 0.0f, 0.1f),
		d = a + glm::vec3(0.0f, 0.0f, 0.1f),
		a1 = a + glm::vec3(0.0f, 0.1f, 0.0f),
		b1 = b + glm::vec3(0.0f, 0.1f, 0.0f),
		c1 = c + glm::vec3(0.0f, 0.1f, 0.0f),
		d1 = d + glm::vec3(0.0f, 0.1f, 0.0f);

		std::vector<GLfloat> result = {
			a.x, a.y, a.z,
			c.x, c.y, c.z,
			b.x, b.y, b.z,

			a.x,a.y,a.z,
			d.x,d.y,d.z,
			c.x,c.y,c.z,

			a.x,a.y,a.z,
			b.x,b.y,b.z,
			b1.x,b1.y,b1.z,

			a.x,a.y,a.z,
			b1.x, b1.y, b1.z,
			a1.x,a1.y,a1.z,

			a.x,a.y,a.z,
			a1.x,a1.y,a1.z,
			d1.x,d1.y,d1.z,

			a.x,a.y,a.z,
			d1.x,d1.y,d1.z,
			d.x,d.y,d.z,

			d.x,d.y,d.z,
			d1.x,d1.y,d1.z,
			c.x,c.y,c.z,

			c.x,c.y,c.z,
			d1.x,d1.y,d1.z,
			c1.x,c1.y,c1.z,

			c.x,c.y,c.z,
			c1.x,c1.y,c1.z,
			b1.x,b1.y,b1.z,

			b.x,b.y,b.z,
			c.x,c.y,c.z,
			b1.x,b1.y,b1.z
	};
	return result;
}
