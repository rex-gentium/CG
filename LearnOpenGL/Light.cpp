#include "Light.h"

const Light Light::DEFAULT = {
	{ 0.0f, 0.0f, 0.0f }, // position
	{ 0.2f, 0.2f, 0.2f }, // ambient
	{ 0.8f, 0.8f, 0.8f }, // diffuse
	{ 1.0f, 1.0f, 1.0f }  // specular
};

const Light Light::SPOTLIGHT = {
	{ 0.0f, 0.0f, 0.0f }, // position
	{ 0.01f, 0.01f, 0.01f }, // ambient
	{ 0.1f, 0.1f, 0.1f }, // diffuse
	{ 1.0f, 1.0f, 1.0f }  // specular
};