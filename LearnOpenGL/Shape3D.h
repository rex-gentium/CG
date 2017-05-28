#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

typedef glm::vec3 Point;
typedef glm::vec2 UV;
typedef glm::vec4 Color;

struct Triangle {
	Point a, b, c;
	UV uv_a, uv_b, uv_c;
	glm::vec3 normal;
	Triangle() {}
	Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {
		normal = glm::normalize(glm::cross(b - a, c - a));
	}
	Triangle(Point a, Point b, Point c, UV uv_a, UV uv_b, UV uv_c) : a(a), b(b), c(c), uv_a(uv_a), uv_b(uv_b), uv_c(uv_c) {
		normal = glm::normalize(glm::cross(b - a, c - a));
	}
	std::vector<Triangle> segment() const {
		std::vector<Triangle> result(4);
		Point ab_middle = 0.5f * (a + b), bc_middle = 0.5f * (b + c), ca_middle = 0.5f * (c + a);
		result[0] = Triangle(a, ab_middle, ca_middle);
		result[1] = Triangle(b, bc_middle, ab_middle);
		result[2] = Triangle(c, ca_middle, bc_middle);
		result[3] = Triangle(ab_middle, bc_middle, ca_middle);
		return result;
	}
	void normalize(Point center, float radius) {
		a = center + radius * glm::normalize(a - center);
		b = center + radius * glm::normalize(b - center);
		c = center + radius * glm::normalize(c - center);
		normal = glm::normalize(glm::cross(b - a, c - a));
	}
	bool contains(Point x) { return a == x || b == x || c == x; }
};

struct Shape {
	std::vector<Triangle> polygons;
	Point position;
	glm::vec3 rotation;
	glm::vec3 scale;
	Color color;

	Shape(std::vector<Triangle> polygons, 
		Point position = { 0.0f, 0.0f, 0.0f }, 
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f },
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f },
		Color color = { 1.0f, 1.0f, 1.0f, 1.0f }) : polygons(polygons), position(position), rotation(rotation), scale(scale), color(color) {}

	GLfloat * getVertexNormalBufferData(int& vertexBufferSize) const {
		vertexBufferSize = polygons.size() * 3 * (3 + 3); // 3 points, 3 coordinates + 3 normal coordinates
		GLfloat * result = new GLfloat[vertexBufferSize];
		for (int i = 0; i < polygons.size(); ++i) {
			result[18 * i] = polygons[i].a.x; result[18 * i + 1] = polygons[i].a.y; result[18 * i + 2] = polygons[i].a.z;
			result[18 * i + 6] = polygons[i].b.x; result[18 * i + 7] = polygons[i].b.y; result[18 * i + 8] = polygons[i].b.z;
			result[18 * i + 12] = polygons[i].c.x; result[18 * i + 13] = polygons[i].c.y; result[18 * i + 14] = polygons[i].c.z;
			result[18 * i + 3] = result[18 * i + 9] = result[18 * i + 15] = polygons[i].normal.x;
			result[18 * i + 4] = result[18 * i + 10] = result[18 * i + 16] = polygons[i].normal.y;
			result[18 * i + 5] = result[18 * i + 11] = result[18 * i + 17] = polygons[i].normal.z;
		}
		return result;
	}

	GLfloat * getVertexUVBufferData(int& vertexBufferSize) const {
		vertexBufferSize = polygons.size() * 3 * (3 + 2); // 3 points, 3 coordinates + 2 uv coordinates
		GLfloat * result = new GLfloat[vertexBufferSize];
		for (int i = 0; i < polygons.size(); ++i) {
			result[15 * i] = polygons[i].a.x; result[15 * i + 1] = polygons[i].a.y; result[15 * i + 2] = polygons[i].a.z;
			result[15 * i + 3] = polygons[i].uv_a.x; result[15 * i + 4] = polygons[i].uv_a.y;
			result[15 * i + 5] = polygons[i].b.x; result[15 * i + 6] = polygons[i].b.y; result[15 * i + 7] = polygons[i].b.z;
			result[15 * i + 8] = polygons[i].uv_b.x; result[15 * i + 9] = polygons[i].uv_b.y;
			result[15 * i + 10] = polygons[i].c.x; result[15 * i + 11] = polygons[i].c.y; result[15 * i + 12] = polygons[i].c.z;
			result[15 * i + 13] = polygons[i].uv_c.x; result[15 * i + 14] = polygons[i].uv_c.y;
		}
		return result;
	}

	Shape * segment(int precision = 1) const {
		if (precision <= 0) return new Shape(polygons, position, rotation, scale, color);
		std::vector<Triangle> segmentedPolygons(polygons.size() * 4);
		for (int i = 0; i < polygons.size(); ++i) {
			auto segments = polygons[i].segment();
			segmentedPolygons[4 * i] = segments[0];
			segmentedPolygons[4 * i + 1] = segments[1];
			segmentedPolygons[4 * i + 2] = segments[2];
			segmentedPolygons[4 * i + 3] = segments[3];
		}
		Shape * result = new Shape(segmentedPolygons, position, rotation, scale, color);
		if (--precision > 0) {
			Shape * temp = result;
			result = temp->segment(precision);
			delete temp;
		}
		return result;
	}

	int getVertexCount() const { return polygons.size() * 3; }

	int getPolygonCount() const { return polygons.size(); }

	void normalize(Point center, float radius) {
		for (int i = 0; i < polygons.size(); ++i)
			polygons[i].normalize(center, radius);
	}

	glm::vec3 getVertexNormal(Point vertex) {
		glm::vec3 result;
		for (int i = 0; i < polygons.size(); ++i)
			if (polygons[i].contains(vertex)) {
				result += polygons[i].normal;
				glm::normalize(result);
			}
		return result;
	}

	void draw(GLint vao, Shader & shader, const glm::mat4 & view, const glm::mat4 & projection) {
		shader.use();
		GLint modelLoc = glGetUniformLocation(shader.program, "model");
		GLint viewLoc = glGetUniformLocation(shader.program, "view");
		GLint projLoc = glGetUniformLocation(shader.program, "projection");
		glm::mat4 model = glm::translate(glm::mat4(), position);
		model = glm::rotate(model, rotation.x, { 1.0f, 0.0f, 0.0f });
		model = glm::rotate(model, rotation.y, { 0.0f, 1.0f, 0.0f });
		model = glm::rotate(model, rotation.z, { 0.0f, 0.0f, 1.0f });
		model = glm::scale(model, scale);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
		glBindVertexArray(0);
	}
};
