#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Hazel/ResourceLoading/shader.h"
#include "Hazel/ResourceLoading/model.h"
#include "Hazel/ResourceLoading/FileSystem.h"
#include "Hazel/Application.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Actors/camera.h"

#include "Level.h"


namespace Hazel
{
	bool Level::firstLevel = true;

    // lights
        // ------
    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f, 0.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(150.0f, 150.0f, 150.0f),
    };
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;


	void Level::Init()
	{
		if (firstLevel) {
			firstLevel = false;

			//add camera
			Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
			m_Actors.emplace_back(camera);

			// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
			stbi_set_flip_vertically_on_load(true);

            Actor* actor;
            actor = new Actor();
            Model* model;
			model = new Model(FileSystem::getPath("resources/objects/3d_other_ue4lcjddw/ue4lcjddw.json"), "PBR");
            actor->models.emplace_back(model);
            m_Actors.emplace_back(actor);


            for (Shader* shader : Application::m_Window->m_Viewport->m_Shaders)
            {
                // initialize static shader uniforms before rendering
                // --------------------------------------------------
                glm::mat4 projection = glm::perspective(
                    glm::radians(camera->Zoom),
                    (float)Application::m_Window->m_Viewport->GetWidth() / (float)Application::m_Window->m_Viewport->GetWidth(),
                    0.1f, 100.0f);

                shader->setMat4("projection", projection);

            }

            //set the light uniforms
            for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
            {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = lightPositions[i];
                
                for (Shader* shader : Application::m_Window->m_Viewport->m_Shaders)
                {
                    if (shader->name == "PBR") 
                    {
                        shader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                        shader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
                    }
                }
            }

		}
		else {

		}
	}

    void Level::OnRender()
    {
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 view = dynamic_cast<Camera*>(Application::m_Window->m_Viewport->currentLevel->m_Actors[0])->GetViewMatrix();//here m_Actors[0] is the camera, should be changed later
        Application::m_Window->m_Viewport->currentShader->setMat4("view", view);
        glm::mat4 modelTrans = glm::mat4(1.0f);
        modelTrans = glm::translate(modelTrans, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        modelTrans = glm::scale(modelTrans, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        Application::m_Window->m_Viewport->currentShader->setMat4("model", modelTrans);
        
        for (Actor* actor : Application::m_Window->m_Viewport->currentLevel->m_Actors)
        {
            for (Model* model : actor->models)
            {
                model->Draw(*Application::m_Window->m_Viewport->currentShader);
            }
        }
        
    }

}
