#pragma once
#include "hzpch.h"

#include <glm.hpp>


namespace Hazel
{
	struct PhysicsProps
	{
		glm::vec3 transform;
		float mass;
		glm::vec3 velocity;
		float rigid;//represents the energy loss in collision
		bool dynamic;
	};


	class CollisionBox
	{
	public:
		CollisionBox() {}
		virtual ~CollisionBox() = default;

	private:
		PhysicsProps props;
	};

	class SphereBox : public CollisionBox
	{
	private:
		float radius;
	};
}


