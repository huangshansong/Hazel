#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/HObject.h"

namespace Hazel
{
	struct HAZEL_API PhysicsProps
	{
		float mass;
		//all relative. model box related to model, mesh box related to mesh
		glm::vec3 centerOfMassTransform;
		glm::vec3 velocity;
		glm::vec3 rotateAxis;
		glm::vec3 rotateSpeed;

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


