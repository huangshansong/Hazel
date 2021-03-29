#pragma once
#include "hzpch.h"

#include "Hazel/ResourceLoading/landscape.h"

#include "Level.h"

namespace Hazel
{
	class HAZEL_API PlantsLevel : public Level
	{
	public:
		PlantsLevel(Level* level, std::string = "Unnamed") : Level(level) {};
		PlantsLevel(void* viewport, std::string = "Unnamed") : Level(viewport) {};

		virtual void onRender() override;

		void gen_PlantsLocation(LandscapeModel*);

		std::vector<std::vector<Actor*>> m_Plants;
	
	};
}


