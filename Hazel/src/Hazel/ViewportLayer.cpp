#include"hzpch.h"

#include <GLFW/glfw3.h>

#include "Core.h"
#include "Log.h"

#include "WindowLayer.h"
#include "ViewportLayer.h"
#include "Application.h"
#include "KeyCode.h"

namespace Hazel {
	void ViewportLayer::onAttach()
	{
		Viewport* viewport = new Viewport;
		WindowLayer::setViewport(Application::getWindow(), viewport);
		Application::getWindow()->getViewport()->init();
		
	}
	void ViewportLayer::onDetach()
	{
		delete Application::getWindow()->getViewport();
	}
	void ViewportLayer::onRender()
	{
		Application::getWindow()->getViewport()->onRender();
	}
	void ViewportLayer::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(ViewportLayer::onFramebufferResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ViewportLayer::onKeyPressed));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ViewportLayer::onMouseMoved));

	}
	void ViewportLayer::onUpdate()
	{
		begin();
		onRender();
		end();
	}

	void ViewportLayer::begin()
	{
		
	}

	void ViewportLayer::end()
	{
	}


	bool ViewportLayer::onFramebufferResize(FramebufferResizeEvent& e) {
		glViewport(0, 0, e.getWidth(), e.getHeight());
		return true;
	}

	bool ViewportLayer::onKeyPressed(KeyPressedEvent& e)
	{
		switch (e.getKeyCode()) {
		case HZ_KEY_ESCAPE: {
			glfwSetInputMode(static_cast<GLFWwindow*>(Application::getWindow()->getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			HZ_CORE_INFO("CURSOR APPEAR!");
			Application::getWindow()->getViewport()->m_CursorCaptured = false;
			break;
		}
		case HZ_KEY_F5: {
			glfwSetInputMode(static_cast<GLFWwindow*>(Application::getWindow()->getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			HZ_CORE_INFO("CURSOR CAPTRUED!");
			Application::getWindow()->getViewport()->m_CursorCaptured = true;
			Application::getWindow()->getViewport()->m_FirstCursor = true;
			break;
		}
		}
		return false;
	}

	bool ViewportLayer::onMouseMoved(MouseMovedEvent& e)
	{
		if (Application::getWindow()->getViewport()->isFirstCursor())
		{
			Application::getWindow()->getViewport()->m_FirstCursor = false;
		}
		Application::getWindow()->getViewport()->m_CursorLastX = e.getX();
		Application::getWindow()->getViewport()->m_CursorLastY = e.getY();

		return false;
	}

}