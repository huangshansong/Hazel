#include "hzpch.h"

#include <GLFW/glfw3.h>

#include "Application.h"
#include "Log.h"

using namespace std;
namespace Hazel 
{
	float Application::s_DeltaTime = 0.0f;
	float Application::s_LastFrameTime = 0.0f;

	Application::Application() 
	{
		//Editor
		Editor* editor = new Editor(this);

		//Window
		Window* window1 = Window::create(this);
		//bind event function of the application to the window events
		window1->setEventCallback(BIND_EVENT_FN(Application::onPlayerInputEvent));

		//just for testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		m_Editor->canDiplayTest(window1);

	}

	void Application::run() 
	{

		//app start
		m_Running = true;
		s_LastFrameTime = 0.0f;
		while (m_Running) {
			
			// per-frame time logic
			// --------------------
			float currentFrameTime = glfwGetTime();
			s_DeltaTime = currentFrameTime - s_LastFrameTime;
			s_LastFrameTime = currentFrameTime;

			m_Running = false;
			for (vector<shared_ptr<Window>>::iterator it = m_Windows.begin(); it != m_Windows.end();)
			{
				if ((*it)->isWindowClosed())
				{
					it = m_Windows.erase(it);
				}
				else 
				{
					it++;
					m_Running = true;
				}
			}

			for (shared_ptr<Window> window : m_Windows)
			{
				window->onUpdate();
				//the playerInputs and GuiInputs will be handled here, see window->onUpdate()'s last ine, it's glfwPollEvents().
			}
			m_Editor->pollEditions();
		}
	}

	void Application::onPlayerInputEvent(Event& event) 
	{
		HZ_CORE_TRACE("{0}", event);
		for (shared_ptr<Window> window : m_Windows)
		{
			window->onPlayerInputEvent(event);
		}	
	}
}