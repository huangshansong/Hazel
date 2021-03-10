#pragma once

namespace Hazel
{
	enum class SimpleGeometryType
	{
		Sphere = 0
	};

	struct Sphere
	{
		float radius;
		Sphere(float radius = 0.0f) : radius(radius){}
	};

}