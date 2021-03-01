#include "hzpch.h"

#include <imgui.h>
#define IMGUI_IMPL_API
#define IMGUI_IMPLE_OPENGL_LOADER_GLAD
#include <examples/imgui_impl_glfw.cpp>
#include <examples/imgui_impl_opengl3.cpp>
//TEMPORARY
#include <GLFW/glfw3.h>
#include <Glad/glad.h>

#include "Hazel/Application.h"

#include "ImGuiLayer.h"

namespace Hazel {

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::m_Window->GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}

	void ImGuiLayer::OnDetach()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void showDemo2() {

		ImGui::SetNextWindowSize({400, 170});

		ImGui::Begin("Shader Selection");                          // Create a window called "Hello, world!" and append into it.

		ImGui::SetWindowFontScale(2.0f);

		ImGui::Text("Click to select the shader.");               // Display some text (you can use a format strings too)

		for (Shader* shader : Application::m_Window->m_Viewport->m_Shaders) 
			if (ImGui::Button(shaderNames[static_cast<int>(shader->name)]))
				Application::m_Window->m_Viewport->currentShader = shader;

		ImGui::SameLine();
		ImGui::Text("\n(%.1f FPS)", ImGui::GetIO().Framerate);

		ImGui::End();
		
	}

	void ImGuiLayer::OnRender()
	{
		static bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);
		showDemo2();
	}

	void ImGuiLayer::OnUpdate()
	{
		Begin();
		OnRender();
		End();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(Application::m_Window->GetWidth(), Application::m_Window->GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}