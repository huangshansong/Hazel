#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ResourceLoading/shader.h"
#include "ResourceLoading/FileSystem.h"

#include "Viewport.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"



namespace Hazel {
    
	void Viewport::Init()
	{
		width = Application::m_Window->GetWidth();
		height = Application::m_Window->GetHeight();
		glViewport(0, 0, width, height);
        
		// configure global opengl state
		glEnable(GL_DEPTH_TEST);
		glfwWindowHint(GLFW_SAMPLES, 4);

		// build and compile shaders
		// -------------------------
		//Shader* shader = new Shader(ShaderName::PBR, FileSystem::getPath("src/1.2.pbr.vs").c_str(), FileSystem::getPath("src/1.2.pbr.fs").c_str());
		Shader* shader = new Shader(ShaderName::PBR, FileSystem::getPath("src/1.2.pbr.vs").c_str(), FileSystem::getPath("src/1.2.pbr.fs").c_str());

		shader->use();
        //push the shader into vector
		m_Shaders.emplace_back(shader);

		/*
        shader = new Shader("Blin-Phong", FileSystem::getPath("src/1.model_loading.vs").c_str(), FileSystem::getPath("src/1.model_loading.fs").c_str());
        shader->use();
        //push the shader into vector
        m_Shaders.emplace_back(shader);
		*/

		//set initial ShaderID, now it's Blin-Phong shader
		currentShader = shader;

	}

	void Viewport::OnRender()
	{
        
        currentShader->use();

	}
	Shader* Viewport::getShader(ShaderName shaderName) const
	{
		for (Shader* shader : m_Shaders)
		{
			if (shader->name == shaderName) return shader;
		}
		return nullptr;
	}
}