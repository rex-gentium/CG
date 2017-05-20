#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Material {
	static const Material DEFAULT, EMERALD, RED_PLASTIC;
	glm::vec3 ambient, // what color this object reflects under ambient lighting; this is usually the same as the object's color
		diffuse, // the color of the object under diffuse lighting
		specular; // the color impact a specular light has on the object
	GLfloat shininess; // the scattering/radius of the specular highlight
};