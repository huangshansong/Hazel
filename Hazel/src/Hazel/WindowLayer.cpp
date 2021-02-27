#include"hzpch.h"

#include <GLFW/glfw3.h>

#include "WindowLayer.h"
#include "Application.h"
#include "Core.h"



namespace Hazel {

	void WindowLayer::OnAttach()
	{
		Application::m_Window = Window::Create();

	}

	void WindowLayer::OnDetach()
	{
		delete Application::m_Window;
	}

	void WindowLayer::OnRender()
	{
		Application::m_Window->OnRender();

	}

	void WindowLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(WindowLayer::OnWindowClose));

	}

	void WindowLayer::OnUpdate()
	{
		Begin();
		OnRender();
		End();
	}

	void WindowLayer::Begin()
	{

	}

	void WindowLayer::End()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(Application::m_Window->GetNativeWindow()));
		glfwPollEvents();
	}

	bool WindowLayer::OnWindowClose(WindowCloseEvent& e) {
		Application::m_WindowClose = true;
		return true;
	}
}