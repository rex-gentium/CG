#include "Material.h"

const Material Material::DEFAULT = {
	{ 1.0f, 0.5f, 0.31f },
	{ 1.0f, 0.5f, 0.31f },
	{ 0.5f, 0.5f, 0.5f },
	8.0f
};

const Material Material::EMERALD = {
	{ 0.0215f, 0.1745f, 0.0215f },
	{ 0.07568f, 0.61424f, 0.07568f },
	{ 0.633f, 0.727811f, 0.633f },
	76.8f
};

const Material Material::RED_PLASTIC = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.5f, 0.0f, 0.0f },
	{ 0.7f, 0.6f, 0.6f },
	32.0f
};