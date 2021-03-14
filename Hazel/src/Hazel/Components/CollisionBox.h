#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/HObject.h"

namespace Hazel
{
	struct HAZEL_API PhysicsProps
	{
		glm::vec3 transform;
		float mass;
		glm::vec3 velocity;
		float rigid;//represents the energy loss in collision
		bool dynamic;
	};


	class HAZEL_API CollisionBox : public HObject
	{
	public:
		CollisionBox() {}
		virtual ~CollisionBox() = default;

		PhysicsProps props;
	};

	class HAZEL_API SphereBox : public CollisionBox
	{
		float radius;
	};
}


