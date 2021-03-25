#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Viewport.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "KeyCode.h"

using namespace std;
namespace Hazel {
	Viewport::Viewport(void* window)
		: m_OfWindow(window)
	{
		((Window*)window)->m_Viewports.emplace_back(shared_ptr<Viewport>(this));
		init(); 
	}
	void Viewport::init()
	{

		m_IsCursorCaptured = false;

		glfwSetInputMode((GLFWwindow*)(((Window*)m_OfWindow)->getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		HZ_CORE_INFO("CURSOR APPEAR!");

	}

	void Viewport::onUpdate()
	{
		for (shared_ptr<Level> rootLevel : m_RootLevels)
		{
			rootLevel->onUpdate();
		}

		//multi-thread processing needed, physics one, and render one

		onRender();
		m_CurrentRootLevel->onRender();
	}

	void Viewport::onRender()
	{
		m_Width = ((Window*)m_OfWindow)->getWidth();
		m_Height = ((Window*)m_OfWindow)->getHeight();

		//should change this for mutiple viewports implementation
		glViewport(0, 0, m_Width, m_Height);

		// render
		// ------
		glEnable(GL_DEPTH_TEST);
		//HZ_CORE_INFO("glEnable(GL_DEPTH_TEST).");
		glfwWindowHint(GLFW_SAMPLES, 4);
		//HZ_CORE_INFO("glfwWindowHint(GLFW_SAMPLES, 4).");
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//HZ_CORE_INFO("glClearColor();");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//HZ_CORE_INFO("glClear();");

	}
	void Viewport::onPlayerInputEvent(Event& event)
	{
		for (shared_ptr<Level> rootLevel : m_RootLevels)
		{
			rootLevel->onPlayerInputEvent(event);
		}
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(Viewport::onFramebufferResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Viewport::onKeyPressed));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Viewport::onMouseMoved));

	}

	bool Viewport::onFramebufferResize(FramebufferResizeEvent& e)
	{
		glViewport(0, 0, e.getWidth(), e.getHeight());
		return true;
	}

	bool Viewport::onKeyPressed(KeyPressedEvent& e)
	{
		switch (e.getKeyCode()) {
		case HZ_KEY_ESCAPE: {
			glfwSetInputMode(static_cast<GLFWwindow*>(((Window*)m_OfWindow)->getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			HZ_CORE_INFO("CURSOR APPEAR!");
			m_IsCursorCaptured = false;
			break;
		}
		case HZ_KEY_F5: {
			glfwSetInputMode(static_cast<GLFWwindow*>(((Window*)m_OfWindow)->getNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			HZ_CORE_INFO("CURSOR CAPTRUED!");
			m_IsCursorCaptured = true;
			m_IsFirstCursor = true;
			break;
		}
		}
		return false;
	}
	bool Viewport::onMouseMoved(MouseMovedEvent& e)
	{
		if (m_IsFirstCursor)
		{
			m_IsFirstCursor = false;

			m_CursorLastX = e.getX();
			m_CursorLastY = e.getY();
			m_CursorOffsetX = 0.0f;
			m_CursorOffsetY = 0.0f;

			return false;
		}
		else
		{

			m_CursorOffsetX = e.getX() - m_CursorLastX;
			m_CursorOffsetY = e.getY() - m_CursorLastY;
			m_CursorLastX = e.getX();
			m_CursorLastY = e.getY();

			return false;
		}
	}
}