#pragma once
#include "hzpch.h"

#include "Hazel/ResourceLoading/shader.h"
#include "Hazel/Levels/Level.h"

namespace Hazel {
	class Viewport
	{
	public:
		void Init();
		void OnRender();
		
		unsigned int GetWidth() const { return width; }
		unsigned int GetHeight() const { return height; }

		bool firstCursor;
		bool cursorCaptured;
		float lastX;
		float lastY;

		std::vector<Shader*> m_Shaders;
		Shader* currentShader;

		Level* currentLevel;

	private:
		unsigned int width;
		unsigned int height;
	};
}
