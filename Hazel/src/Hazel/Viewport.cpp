#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Viewport.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"


namespace Hazel {
    
	void Viewport::init()
	{
		m_Width = Application::getWindow()->getWidth();
		m_Height = Application::getWindow()->getHeight();
		glViewport(0, 0, m_Width, m_Height);

		m_CursorCaptured = false;

		// configure global opengl state
		glEnable(GL_DEPTH_TEST);
		HZ_CORE_INFO("glEnable(GL_DEPTH_TEST).");
		glfwWindowHint(GLFW_SAMPLES, 4);
		HZ_CORE_INFO("glfwWindowHint(GLFW_SAMPLES, 4).");
		glfwSetInputMode(static_cast<GLFWwindow*>(Application::getWindow()->getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		HZ_CORE_INFO("CURSOR APPEAR!");

	}

	void Viewport::onRender()
	{
		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//HZ_CORE_INFO("glClearColor();");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//HZ_CORE_INFO("glClear();");

	}
}