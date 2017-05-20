#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Light {
	static const Light DEFAULT, SPOTLIGHT;
	glm::vec3 position, ambient, diffuse, specular;
};