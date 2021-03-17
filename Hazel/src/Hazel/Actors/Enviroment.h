#pragma once

#include "Actor.h"

namespace Hazel 
{
	class Enviroment : public Actor
	{
	public:
		Enviroment(void* level, std::string name = "Unnamed", glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f));

		virtual void onRender()const override;

	protected:
		
	};
}


