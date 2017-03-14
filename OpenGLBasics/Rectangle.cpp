#include "Rectangle.h"
#include <glm\gtx\rotate_vector.hpp>


Rectangle::Rectangle(Window * window, ScreenPoint coordinate, int width, int height) : DrawObject2D(window)
{
	point = coordinate;

	this->width = width;
	this->height = height;
	pointRelative = window->screenToRelative(point);
	widthRelative = width / (float)window->getScreenWidth();
	heightRelative = height / (float)window->getScreenHeight();
	vertexCount = 4;
	vertices = new glm::vec2[vertexCount];
	updateVertices();
}

Rectangle::Rectangle(Window * window, RelativePoint coordinate, float width, float height): DrawObject2D(window)
{
	point = window->relativeToScreen(coordinate);
	pointRelative = coordinate;
	this->width = static_cast<int>(width * window->getScreenWidth());
	this->height = static_cast<int>(height * window->getScreenHeight());
	this->widthRelative = width;
	this->heightRelative = height;
	vertexCount = 4;
	vertices = new glm::vec2[vertexCount];
	updateVertices();
}

Rectangle::~Rectangle()
{
}

void Rectangle::draw()
{
	if (lockPosition)
		pointRelative = window->screenToRelative(point);
	if (lockProportion)
		heightRelative = window->getScreenRatio() * widthRelative;
	if (lockSize) {
		widthRelative = width / (float)window->getScreenWidth();
		heightRelative = height / (float)window->getScreenHeight();
	}
	updateVertices();
	// An array of 6 vectors which represents 2 triangles
	GLfloat gVertexBufferData[] = {
		vertices[0].x, vertices[0].y, 0.0f,
		vertices[1].x, vertices[1].y, 0.0f,
		vertices[2].x, vertices[2].y, 0.0f,
		vertices[0].x, vertices[0].y, 0.0f,
		vertices[2].x, vertices[2].y, 0.0f,
		vertices[3].x, vertices[3].y, 0.0f,
	};
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
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

void Rectangle::updateVertices()
{
	vertices[0] = { pointRelative.x, pointRelative.y };
	vertices[1] = { pointRelative.x + widthRelative, pointRelative.y };
	vertices[2] = { pointRelative.x + widthRelative, pointRelative.y - heightRelative };
	vertices[3] = { pointRelative.x, pointRelative.y - heightRelative };
	// translation

	// rotation
	glm::vec3 center((vertices[0].x + vertices[2].x) / 2.0f, (vertices[0].y + vertices[2].y) / 2.0f, 0.0f);
	for (int i = 0; i < vertexCount; ++i) {
		glm::mat3x2 translationMatrix(0.0f);
		translationMatrix[0][0] = 1.0f;
		translationMatrix[1][1] = 1.0f;
		translationMatrix[2][0] = -center.x;
		translationMatrix[2][1] = -center.y;
		vertices[i] = translationMatrix * glm::vec3(vertices[i].x, vertices[i].y, 1.0f);
		vertices[i] = glm::rotate(vertices[i], rotation);
	}
}
