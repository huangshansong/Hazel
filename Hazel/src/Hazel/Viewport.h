#pragma once
#include "hzpch.h"

#include "Hazel/Levels/Level.h"
#include "Hazel/Levels/LevelLayer.h"

namespace Hazel {
	class Viewport
	{
		friend class ViewportLayer;
	public:
		
		const unsigned int getWidth() const { return m_Width; }
		const unsigned int getHeight() const { return m_Height; }
		const bool isFirstCursor() const { return m_FirstCursor; }
		const bool isCursorCaptured() const { return m_CursorCaptured; }
		const float getCursorLastX() const { return m_CursorLastX; }
		const float getCursorLastY() const { return m_CursorLastY; }
		Level* getLevel() { return m_CurrentLevel; }

	protected:
		void init();
		void onRender();

		Level* m_CurrentLevel;
		unsigned int m_Width;
		unsigned int m_Height;
		bool m_FirstCursor;
		bool m_CursorCaptured;
		float m_CursorLastX;
		float m_CursorLastY;
	};
}
