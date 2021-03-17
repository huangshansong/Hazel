#include "hzpch.h"

#include "Hazel/Log.h"

#include "HazelMathLib.h"

namespace Hazel
{
	float normalDistributionFunc(float x, float mean, float variance)
	{
		variance = variance > 1E-5f ? variance : 1E-5f;
		float temp = std::exp(- ((x - mean) * (x - mean)) / (2 * variance * variance)) / (sqrtf(2.0f * std::_Pi) * variance);
		temp = temp > 1E-5f ? temp : 1E-5f;
		return temp;
	}

	float heightInteractionFunc(float h1, float h2)
	{
		if (h1 > h2) std::swap(h1, h2);
		h1 = h1 > 1E-5f ? h1 : 1E-5f;
		h2 = h2 > 1E-5f ? h2 : 1E-5f;
		float x = h2 / h1;

		
		float temp = std::powf(x, 2.5);
		temp = temp > 1E-10f ? temp : 1E-10f;
		
		temp = h2 + h1 * (2.5 * std::log(x) / temp);
		temp = temp > 1E-5f ? temp : 1E-5f;

		//float temp = h2 + h1 * (0.1 / x);

		return temp;
	}
	float areaOfTriangle(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3)
	{
		float a = glm::distance(P1, P2);
		float b = glm::distance(P1, P3);
		float c = glm::distance(P2, P3);
		float p = (a + b + c) / 2;
		return std::sqrtf(p * (p - a) * (p - b) * (p - c));
	}
}

