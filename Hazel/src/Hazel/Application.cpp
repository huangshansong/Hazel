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

	Window* Application::s_Window = nullptr;
	float Application::s_DeltaTime = 0.0f;
	float Application::s_LastFrameTime = 0.0f;
	bool Application::s_CanDisplayTest = true;

	Application::Application() {

		//Window
		WindowLayer* windowLayer = new WindowLayer;
		pushLayer(windowLayer);
		//bind event function of the application to the window events
		WindowLayer::setEventCallback(Application::s_Window, BIND_EVENT_FN(Application::onEvent));

		//Viewport
		ViewportLayer* viewportLayer = new ViewportLayer;
		pushLayer(viewportLayer);

		//Level, the first level
		s_CanDisplayTest = false;
		LevelLayer* levelLayer = new LevelLayer;
		//if you push a level, it will be the current level
		pushLayer(levelLayer);

		//GUI
		ImGuiLayer* imGuiLayer = new ImGuiLayer;
		pushOverlay(imGuiLayer);

		
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer) {
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::run() {

		//app start
		m_Running = true;
		s_LastFrameTime = 0.0f;
		while (m_Running) {
			
			// per-frame time logic
			// --------------------
			float currentFrameTime = glfwGetTime();
			s_DeltaTime = currentFrameTime - s_LastFrameTime;
			s_LastFrameTime = currentFrameTime;

			//render and update
			for (Layer* layer : m_LayerStack) {
				layer->onUpdate();
			}
			
			//app needs to be closed when window is closed
			m_Running = !s_Window->isWindowClosed();
		}

		//backward close all layers
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onDetach();
		}
	}

	void Application::onEvent(Event& e) {
		
		
		HZ_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.m_Handled)
				break;
		}
	}

}