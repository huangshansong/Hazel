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

		float temp = std::powf(x, 5);
		temp = temp > 1E-10f ? temp : 1E-10f;

		temp = h2 + h1 * (5 * std::log(x) / temp);
		temp = temp > 1E-5f ? temp : 1E-5f;

		return temp;
	}
}

