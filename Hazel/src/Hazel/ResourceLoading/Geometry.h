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
	struct HAZEL_API Cube : public HObject
	{
		float length;
		Cube(float length = 1.0f) : length(length) {}
	};
	struct HAZEL_API Quad : public HObject
	{
		float length;
		Quad(float length = 1.0f) : length(length) {}
	};
	
}