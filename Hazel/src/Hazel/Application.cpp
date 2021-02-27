#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGui/ImGuiLayer.h"
#include "Hazel/Levels/LevelLayer.h"

#include "Application.h"
#include "Log.h"
#include "WindowLayer.h"
#include "ViewportLayer.h"


namespace Hazel {

	Window* Application::m_Window = nullptr;
	float Application::deltaTime = 0.0f;
	bool Application::m_WindowClose = false;

	Application::Application() {

		//app start
		m_Running = true;

		// timing
		lastFrameTime = 0.0f;

		//Window
		WindowLayer* windowLayer = new WindowLayer();
		PushLayer(windowLayer);
	
		//Viewport
		ViewportLayer* viewportLayer = new ViewportLayer();
		PushLayer(viewportLayer);

		//Level, the first level
		LevelLayer* levelLayer = new LevelLayer();
		PushLayer(levelLayer);

		//GUI
		ImGuiLayer* imGuiLayer = new ImGuiLayer();
		PushOverlay(imGuiLayer);

		//bind event function of the application to the window events
		Application::m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run() {

		while (m_Running) {
			
			// per-frame time logic
			// --------------------
			float currentFrameTime = glfwGetTime();
			deltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			//render and update
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
			
			//app need to be closed
			m_Running = !m_WindowClose;
		}

		//backward close all layers
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnDetach();
		}
	}

	void Application::OnEvent(Event& e) {
		
		
		HZ_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

}