#pragma once

#include <glm.hpp>

namespace Hazel
{
	float normalDistributionFunc(float x, float mean = 0.0f, float variance = 1.0f);
	
	float heightInteractionFunc(float h1, float h2);

	float areaOfTriangle(glm::vec3, glm::vec3, glm::vec3);
}