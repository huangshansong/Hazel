#pragma once
#include "hzpch.h"

#include "Hazel/ResourceLoading/shader.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Actors/Camera.h"

namespace Hazel 
{
	class Level
	{
		friend class LevelLayer;
	public:
		
	private:
		
		void init();
		void onRender();
		
		void gen_ItemLocation();

		std::vector<Actor*> m_Actors;
		std::vector<Shader*> m_Shaders;

		static bool m_FirstLevel;

	};

}

