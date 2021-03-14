#include "hzpch.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Log.h"
#include "Hazel/Application.h"

#include "WindowsWindow.h"

using namespace std;
namespace Hazel {
	
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(void* application, const WindowProps& props)
	{
		m_OfApplication = application;
		((Application*)application)->m_Windows.emplace_back(shared_ptr<Window>(this));
		init(props);
		m_WindowClosed = false;

		//GUI
		MyImGui* imGui = new MyImGui(this);

	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;


		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			HZ_CORE_INFO("glfwInit() successfully called.");

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		if (m_Window == NULL)
		{
			glfwTerminate();
			HZ_CORE_ASSERT(1, "Failed to create GLFW window");	
		}
		else
		{
			HZ_CORE_INFO("glfwCreateWindow() successfully called.");
		}

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		HZ_CORE_INFO("gladLoadGLLoader() successfully called.");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);

		
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow * window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			FramebufferResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
		
	}

	void WindowsWindow::shutdown()
	{
		m_ImGui = nullptr;

		glfwDestroyWindow(m_Window);

		//when the deconstructor is called, the vector still has this element, so here is 1, not 0
		if (((Application*)m_OfApplication)->m_Windows.size() == 1)
		{
			glfwTerminate();
		}
	}


	void WindowsWindow::onRender()
	{
		
	}

	void WindowsWindow::onPlayerInputEvent(Event& event)
	{
		for (shared_ptr<Viewport> viewport : m_Viewports)
		{
			viewport->onPlayerInputEvent(event);
		}
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(WindowsWindow::onWindowClose));

	}

	bool WindowsWindow::onWindowClose(WindowCloseEvent& event)
	{
		m_WindowClosed = true;
		return true;
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::onUpdate()
	{
		
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		for (shared_ptr<Viewport> viewport : m_Viewports)
		{
			viewport->onUpdate();
		}

		m_ImGui->onUpdate();//this will probably edit something

		glfwSwapBuffers(m_Window);
		glfwPollEvents();//contains player inputs and Gui inputs in this window
		
	}

}
