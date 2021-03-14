#pragma once
#include "hzpch.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Levels/Level.h"

#include "HObject.h"

namespace Hazel {
	class HAZEL_API Viewport : public HObject
	{
	public:
		Viewport(void* window);

		~Viewport() = default;

		unsigned int getWidth() const { return m_Width; }

		unsigned int getHeight() const { return m_Height; }

		bool isFirstCursor() const { return m_IsFirstCursor; }

		bool isCursorCaptured() const { return m_IsCursorCaptured; }

		float getCursorLastX() const { return m_CursorLastX; }

		float getCursorLastY() const { return m_CursorLastY; }

		float getCursorOffsetX() const { return m_CursorOffsetX; }

		float getCursorOffsetY() const { return m_CursorOffsetY; }

		const void* getOfWindow() const { return m_OfWindow; }

		const Level* getCurrentRootLevel() const { return m_CurrentRootLevel; }

		void setCurrentRootLevel(Level* level) { m_CurrentRootLevel = level; }

		void init();

		void onUpdate();

		void onRender();

		void onPlayerInputEvent(Event&);

		std::vector<std::shared_ptr<Level>> m_RootLevels;

	protected:
		//one viewport RENDER only one rootLevel and its childLevels, UPDATE all rootLevels and their childLevels
		Level* m_CurrentRootLevel = nullptr;

		void* m_OfWindow = nullptr;
		unsigned int m_Width;
		unsigned int m_Height;
		bool m_IsFirstCursor;
		bool m_IsCursorCaptured;
		float m_CursorLastX;
		float m_CursorLastY;
		float m_CursorOffsetX;
		float m_CursorOffsetY;

	private:
		bool onFramebufferResize(FramebufferResizeEvent&);

		bool onKeyPressed(KeyPressedEvent&);

		bool onMouseMoved(MouseMovedEvent&);
	};
}
