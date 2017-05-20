#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Material.h"

typedef glm::vec3 Point;

struct Triangle {
	Point a, b, c;
	glm::vec3 normal;
	Triangle() {}
	Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {
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

class Shape {
	std::vector<Triangle> polygons;
	Material material;
public:
	Shape(std::vector<Triangle> polygons, Material material = Material::DEFAULT) : polygons(polygons), material(material) {}

	GLfloat * getVertexBufferData(int& vertexBufferSize) const {
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

	Shape * segment(int precision = 1) const {
		if (precision <= 0) return new Shape(this->polygons, this->material);
		std::vector<Triangle> segmentedPolygons(polygons.size() * 4);
		for (int i = 0; i < polygons.size(); ++i) {
			auto segments = polygons[i].segment();
			segmentedPolygons[4 * i] = segments[0];
			segmentedPolygons[4 * i + 1] = segments[1];
			segmentedPolygons[4 * i + 2] = segments[2];
			segmentedPolygons[4 * i + 3] = segments[3];
		}
		Shape * result = new Shape(segmentedPolygons, material);
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

	void setMaterial(Material m) { material = m; }
	Material getMaterial() const { return material; }
};
