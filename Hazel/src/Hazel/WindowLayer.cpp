#include"hzpch.h"

#include <GLFW/glfw3.h>

#include "WindowLayer.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"



namespace Hazel {
	
	void WindowLayer::onAttach()
	{
		Window* window = Window::create();
		Application::WindowSetter::setWindow(window);
	}

	void WindowLayer::onDetach()
	{
		delete Application::getWindow();

		glfwTerminate();
		HZ_CORE_INFO("glfwTerminate();");
	}

	void WindowLayer::onRender()
	{
		Application::getWindow()->onRender();

	}

	void WindowLayer::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(WindowLayer::onWindowClose));

	}

	void WindowLayer::onUpdate()
	{
		begin();
		onRender();
		end();
	}

	void WindowLayer::begin()
	{

	}

	void WindowLayer::end()
	{
		glfwSwapBuffers(static_cast<GLFWwindow*>(Application::getWindow()->getNativeWindow()));
		//HZ_CORE_INFO("glfwSwapBuffers(static_cast<GLFWwindow*>(Application::getWindow()->GetNativeWindow()));");
		glfwPollEvents();
		//HZ_CORE_INFO("glfwPollEvents();");
	}

	bool WindowLayer::onWindowClose(WindowCloseEvent& e) {
		Application::WindowCloseSetter::setWindowClose(true);
		return true;
	}
}