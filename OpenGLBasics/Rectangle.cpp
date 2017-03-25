#include "Rectangle.h"
#include <glm\gtx\rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

Rectangle::Rectangle(Window * window, ScreenCoord coordinate, int width, int height) : DrawObject2D(window)
{
	position = coordinate;
	this->width = width;
	this->height = height;
}

Rectangle::~Rectangle()
{
}

void Rectangle::draw()
{
	auto vertices = calculateVertices();
	// An array of 6 vectors which represents 2 triangles
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

std::vector<GLfloat> Rectangle::calculateVertices()
{
	// сначала считаются экранная позиция фигуры
	glm::vec2 verticesScr[4] = {
		{ position.x, position.y },
		{ position.x + width, position.y },
		{ position.x + width, position.y + height },
		{ position.x, position.y + height }
	};
	glm::vec2 center((verticesScr[0].x + verticesScr[1].x) / 2.0f, (verticesScr[0].y + verticesScr[2].y) / 2.0f);
	for (int i = 0; i < 4; ++i) {
		verticesScr[i] = glm::translate(glm::mat3(), -center) * glm::vec3(verticesScr[i].x, verticesScr[i].y, 1.0f);
		verticesScr[i] = glm::rotate(glm::mat3(), rotation) * glm::vec3(verticesScr[i].x, verticesScr[i].y, 1.0f);
		verticesScr[i] = glm::translate(glm::mat3(), center) * glm::vec3(verticesScr[i].x, verticesScr[i].y, 1.0f);
	}
	glm::vec2 verticesNorm[4] = {
		glm::vec2(window->screenXToNormalizedX(verticesScr[0].x), window->screenYToNormalizedY(verticesScr[0].y)),
		glm::vec2(window->screenXToNormalizedX(verticesScr[1].x), window->screenYToNormalizedY(verticesScr[1].y)),
		glm::vec2(window->screenXToNormalizedX(verticesScr[2].x), window->screenYToNormalizedY(verticesScr[2].y)),
		glm::vec2(window->screenXToNormalizedX(verticesScr[3].x), window->screenYToNormalizedY(verticesScr[3].y))
	};
	// в результирующий массив проталкивается 6 точек (2 треугольника) по 3 координаты
	std::vector<GLfloat> result = {
		verticesNorm[0].x, verticesNorm[0].y, 0.0f,
		verticesNorm[1].x, verticesNorm[1].y, 0.0f,
		verticesNorm[2].x, verticesNorm[2].y, 0.0f,
		verticesNorm[0].x, verticesNorm[0].y, 0.0f,
		verticesNorm[2].x, verticesNorm[2].y, 0.0f,
		verticesNorm[3].x, verticesNorm[3].y, 0.0f
	};
	return result;
}
