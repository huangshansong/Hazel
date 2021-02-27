#pragma once
#include "hzpch.h"

#include "Hazel/Actors/Actor.h"

namespace Hazel 
{
	class Level
	{
	public:
		
		void Init();
		void OnRender();
		
		static bool firstLevel;

		std::vector<Actor*> m_Actors;

	};

}

