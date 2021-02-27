#include"hzpch.h"

#include <GLFW/glfw3.h>

#include "ViewportLayer.h"
#include "Application.h"
#include "KeyCode.h"

namespace Hazel {
	void ViewportLayer::OnAttach()
	{
		Application::m_Window->m_Viewport = new Viewport();
		Application::m_Window->m_Viewport->Init();
		
	}
	void ViewportLayer::OnDetach()
	{
		delete Application::m_Window->m_Viewport;
	}
	void ViewportLayer::OnRender()
	{
		Application::m_Window->m_Viewport->OnRender();
	}
	void ViewportLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(ViewportLayer::OnFramebufferResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ViewportLayer::OnKeyPressed));

	}
	void ViewportLayer::OnUpdate()
	{
		Begin();
		OnRender();
		End();
	}

	void ViewportLayer::Begin()
	{
		
	}

	void ViewportLayer::End()
	{
	}


	bool ViewportLayer::OnFramebufferResize(FramebufferResizeEvent& e) {
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}

	bool ViewportLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode()) {
		case HZ_KEY_ESCAPE: {
			glfwSetInputMode(static_cast<GLFWwindow*>(Application::m_Window->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			HZ_CORE_INFO("CURSOR SET!");
			Application::m_Window->m_Viewport->cursorCaptured = false;
			break;
		}
		case HZ_KEY_F5: {
			glfwSetInputMode(static_cast<GLFWwindow*>(Application::m_Window->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Application::m_Window->m_Viewport->cursorCaptured = true;
			Application::m_Window->m_Viewport->firstCursor = true;
			break;
		}
		}
		return false;
	}

}