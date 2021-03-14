#include "hzpch.h"

#include <imgui.h>
#define IMGUI_IMPL_API
#define IMGUI_IMPLE_OPENGL_LOADER_GLAD
#include <examples/imgui_impl_glfw.cpp>
#include <examples/imgui_impl_opengl3.cpp>
//TEMPORARY
#include <GLFW/glfw3.h>
#include <Glad/glad.h>

#include "Hazel/Window.h"
#include "Hazel/Editor/Editor.h"

#include "MyImGui.h"

namespace Hazel {

	MyImGui::MyImGui(void* window)
		: m_OfWindow(window)
	{
		((Window*)window)->m_ImGui = std::unique_ptr<MyImGui>(this);

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
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)(((Window*)m_OfWindow)->getNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}

	MyImGui::~MyImGui()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void showDemo2() {

		ImGui::SetNextWindowSize({220, 100});

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::SetWindowFontScale(2.0f);

		ImGui::Text("\n(%.1f FPS)", ImGui::GetIO().Framerate);

		ImGui::End();
		
	}

	void MyImGui::onRender()
	{

		showDemo2();
	}

	void MyImGui::onUpdate()
	{
		begin();
		onRender();
		end();
	}

	void MyImGui::begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void MyImGui::end()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(((Window*)m_OfWindow)->getWidth(), ((Window*)m_OfWindow)->getHeight());

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