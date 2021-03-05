#pragma once
#include "hzpch.h"

#include "Hazel/Levels/Level.h"
#include "Hazel/Levels/LevelLayer.h"
#include "Hazel/Actors/Camera.h"

namespace Hazel {
	class Viewport
	{
		friend class ViewportLayer;
	public:
		
		unsigned int getWidth() const { return m_Width; }
		unsigned int getHeight() const { return m_Height; }
		inline Camera* getCamera() const { return m_CurrentCamera; }
		inline Level* getLevel() const { return m_CurrentLevel; }
		
		//these are too often used, so I set these to public
		bool m_FirstCursor;
		bool m_CursorCaptured;
		float m_LastX;
		float m_LastY;

		class CameraSetter
		{
			friend class Level;
		private:
			inline static void setCamera(Viewport* viewport, Camera* camera) { viewport->m_CurrentCamera = camera; }
		};
		class LevelSetter
		{
			friend class LevelLayer;
		private:
			inline static void setLevel(Viewport* viewport, Level* level) { viewport->m_CurrentLevel = level; }
		};


	private:
		void init();
		void onRender();

		inline void setCamera(Camera* camera) { m_CurrentCamera = camera; }
		inline void setLevel(Level* level) { m_CurrentLevel = level; }

		unsigned int m_Width;
		unsigned int m_Height;

		Level* m_CurrentLevel = nullptr;
		Camera* m_CurrentCamera = nullptr;
	};
}
