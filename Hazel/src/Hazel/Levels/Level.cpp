#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Hazel/ResourceLoading/shader.h"
#include "Hazel/ResourceLoading/mesh.h"
#include "Hazel/ResourceLoading/model.h"
#include "Hazel/ResourceLoading/FileSystem.h"
#include "Hazel/Application.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Actors/camera.h"
#include "Hazel/Actors/landscape.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"

#include "Level.h"


namespace Hazel
{
	bool Level::m_FirstLevel = true;

    // lights
        // ------
    glm::vec3 lightPositions[] = {
        glm::vec3(0.0f, 0.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(150.0f, 150.0f, 150.0f),
    };
    

	void Level::init()
	{
		if (m_FirstLevel) {
			m_FirstLevel = false;

			//add camera
			Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 1.0f));

            Viewport::CameraSetter::setCamera(Application::getWindow()->getViewport(), camera);
            
			m_Actors.emplace_back(camera);


            Actor* actor;
            Model* model;
            Shader* shader;
            if (Application::getCanDisplayTest())
            {
                stbi_set_flip_vertically_on_load(false);//the backpack model don't need to flip texture
                model = new Model(FileSystem::getPath("resources/objects/backpack/backpack.obj"), false);
                HZ_CORE_INFO("model = new Model()");
                shader = new Shader(ShaderType::Blinn_Phong, FileSystem::getPath("src/blinn_phong_diffuse.vs").c_str(), FileSystem::getPath("src/blinn_phong_diffuse.fs").c_str());
                HZ_CORE_INFO("shader = new Shader()");
                m_Shaders.emplace_back(shader);
                
                for (Mesh* mesh : model->m_Meshes)
                {
                    Mesh::ShaderSetter::setShader(mesh, shader);
                }
                actor = new Actor;
                actor->setModel(model);
                m_Actors.emplace_back(actor);

            }
            else 
            {
                // tell stb_image.h to flip loaded texture's on the y-axis.
                stbi_set_flip_vertically_on_load(true);

                Landscape* landscape = new Landscape;
                landscape->gen_Landscape(1000, 1000);
                landscape->getMaterialsPaths().emplace_back(FileSystem::getPath("resources/surfaces/grass_wild_sgmkajak/"));
                landscape->getMaterialsPaths().emplace_back(FileSystem::getPath("resources/surfaces/ground_other_vbljegefw/"));
                landscape->getMaterialsPaths().emplace_back(FileSystem::getPath("resources/surfaces/moss_ground_umilae2n/"));
                landscape->getMaterialsPaths().emplace_back(FileSystem::getPath("resources/surfaces/rock_rough_vctkajjg/"));
                landscape->getMaterialsPaths().emplace_back(FileSystem::getPath("resources/surfaces/snow_pure_uephfgudy/"));
                model = new Model(landscape);
                // build and compile shaders for every mesh
                // -------------------------
                shader = new Shader(ShaderType::PBR, FileSystem::getPath("src/1.2.pbr.vs").c_str(), FileSystem::getPath("src/1.2.pbr.fs").c_str());
                shader->use();
                HZ_CORE_INFO("shader->use();");
                shader->setFloat("heightScale", 1.0f);
                HZ_CORE_INFO("shader->setFloat(heightScale);");
                m_Shaders.emplace_back(shader);
                //Shader* shader = new Shader(ShaderName::PBR, FileSystem::getPath("src/PBR_albedo.vs").c_str(), FileSystem::getPath("src/PBR_albedo.fs").c_str());

                for (Mesh* mesh : model->m_Meshes)
                {
                    Mesh::ShaderSetter::setShader(mesh, shader);
                }
                actor = new Actor;
                actor->setModel(model);
                m_Actors.emplace_back(actor);

            }
            

            for (Shader* shader : m_Shaders)
            {
                // initialize static shader uniforms before rendering
                // --------------------------------------------------
                glm::mat4 projection = glm::perspective(
                    glm::radians(camera->getZoom()),
                    (float)Application::getWindow()->getViewport()->getWidth() / (float)Application::getWindow()->getViewport()->getWidth(),
                    0.1f, 100.0f);

                shader->use();
                HZ_CORE_INFO("shader->use();");
                shader->setMat4("projection", projection);
                HZ_CORE_INFO("shader->setMat4(projection);");

            }

            //set the light uniforms
            for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
            {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = lightPositions[i];
                
                for (Shader* shader : m_Shaders)
                {
                    if (shader->m_ShaderType == ShaderType::PBR) 
                    {
                        //shader->use();
                        //HZ_CORE_INFO("shader->use();");
                        //shader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                        //shader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
                    }
                }
            }
            for (Shader* shader : m_Shaders)
            {
                if (shader->m_ShaderType == ShaderType::PBR)
                {
                    shader->use();
                    HZ_CORE_INFO("shader->use();");
                    shader->setVec3("lightColor", glm::vec3(150.0f, 150.0f, 150.0f));
                    HZ_CORE_INFO("shader->setVec3(lightColor);");
                    shader->setVec3("lightDirection", glm::vec3(1.0f, 1.5f, 2.0f));
                    HZ_CORE_INFO("shader->setVec3(lightDirection);");
                }
            }

		}
		else {

		}
	}

    void Level::onRender()
    {
        
        glm::mat4 view = Application::getWindow()->getViewport()->getCamera()->getViewMatrix();
        glm::vec3 camPos = Application::getWindow()->getViewport()->getCamera()->getCameraPosition();
        for (Shader* shader : m_Shaders)
        {
            shader->use();
            //HZ_CORE_INFO("shader->use();");
            shader->setMat4("view", view);
            //HZ_CORE_INFO("shader->setMat4(view);");
            shader->setVec3("camPos", camPos);
            //HZ_CORE_INFO("shader->setsetVec3(camPos);");
        }
        
    }

    void Level::gen_ItemLocation()
    {
    }

}
