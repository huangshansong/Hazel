#pragma once

#include "Actor.h"

namespace Hazel 
{
	class Enviroment : public Actor
	{
	public:
		Enviroment(void* level, std::string name = "Unnamed");

		virtual void onRender()const override;

	protected:
		
	};
}


