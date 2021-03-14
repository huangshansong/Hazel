#pragma once

#include "Hazel/HObject.h"

namespace Hazel
{
	enum class SimpleGeometryType
	{
		Sphere = 0
	};

	struct HAZEL_API Sphere : public HObject
	{
		float radius;
		Sphere(float radius = 1.0f) : radius(radius){}
	};

}