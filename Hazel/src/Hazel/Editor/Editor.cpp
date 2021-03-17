#include "hzpch.h"

#include "Hazel/Application.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/ResourceLoading/_ResourceLoading.h"
#include "Hazel/Levels/PlantsLevel.h"
#include "Hazel/Core.h"
#include "Platform/Windows/OpenGLTexture.h"

#include "Editor.h"

using namespace std;
namespace Hazel
{
	Editor::Editor(void* application)
		: m_OfApplication(application)
	{
		((Application*)m_OfApplication)->m_Editor = unique_ptr<Editor>(this);
	}
	void Editor::pollEditions()
	{
		for (EditorCreateCallback& callback : createCallbacks)
		{
			callback();
		}
		for (EditorSetCallback& callback : setCallbacks)
		{
			callback();
		}
		for (EditorDestroyCallback& callback : destroyCallbacks)
		{
			callback();
		}
		createCallbacks.clear();
		setCallbacks.clear();
		destroyCallbacks.clear();
	}
	void Editor::canDiplayTest(Window* window)
    {
        Viewport* viewport = new Viewport(window);
        Level* rootLevel = new Level(viewport);
		viewport->setCurrentRootLevel(rootLevel);
        Camera* camera = new Camera(rootLevel, "main camera");
        Level::setCamera(rootLevel, camera);

		addDefaultMap(rootLevel);
		
    }
    void Editor::addDefaultMap(Level* level)
	{
		addDefaultActor(level, DefaultActor::enviroment);
		//addDefaultActor(level, DefaultActor::container);
		//addDefaultActor(level, DefaultActor::backpack);
		//addDefaultActor(level, DefaultActor::landscape);
		addDefaultActor(level, DefaultActor::sphere);
		//addDefaultActor(level, DefaultActor::grass);
		//addDefaultActor(level, DefaultActor::cerberus); //can't read the texture type by assimp!!
   
	}
	void Editor::addDefaultActor(Level* level, DefaultActor actor)
	{
		if (actor == DefaultActor::enviroment)
		{
			Enviroment* enviroment = new Enviroment(level, "Enviroment");
			level->setEnviroment(enviroment);
			CubeModel* model = new CubeModel(enviroment);
			EnvironmentMaterial* material = new EnvironmentMaterial("resources/textures/skybox/", FileSuffix::jpg);
			model->setModelUniversalMaterial(shared_ptr<Material>(material));
			Shader* shader = new Shader("resources/textures/IBL/");
			model->setModelUniversalShader(shared_ptr<Shader>(shader));

		}
		else if (actor == DefaultActor::container)
		{
			Actor* container = new Actor(level, "Container");
			Model* model = new QuixelModel(container, "resources/3d/container_barrel_udlmcdhqx/", QuixelObjectType::_3d);
			model->setScale(glm::vec3(0.1f));
		}
		else if (actor == DefaultActor::cerberus)
		{
			Actor* cerberus = new Actor(level, "Cerberus");
			Model* model = new LearnOpenGLModel(cerberus, "resources/objects/Cerberus/", FileSuffix::fbx);
		}
		else if (actor == DefaultActor::backpack)
		{
			Actor* backpack = new Actor(level, "Backpack");
			Model* model = new LearnOpenGLModel(backpack, "resources/objects/backpack/", FileSuffix::obj);
		}
		else if (actor == DefaultActor::landscape)
		{
			Actor* landscape = new Actor(level, "Landscape");
			Model* model = new LandscapeModel(landscape);
			model->setScale(glm::vec3(5.0f));
			vector<string> paths;
			paths.emplace_back("resources/surfaces/rock_rough_vctkajjg/");
			paths.emplace_back("resources/surfaces/snow_pure_uephfgudy/");
			QuixelMaterial* material = new QuixelMaterial(paths, QuixelObjectType::_surface, Resolution::_2K);
			model->setModelUniversalMaterial(shared_ptr<Material>(material));
			Shader* shader = new Shader("resources/surfaces/landscape/");
			model->setModelUniversalShader(shared_ptr<Shader>(shader));
		}
		else if (actor == DefaultActor::sphere)
		{
			Actor* sphere = new Actor(level, "Sphere");
			Model* model = new SphereModel(sphere);
			OrdinaryMaterial* material = new OrdinaryMaterial("resources/textures/pbr/gold/", FileSuffix::png);
			model->setModelUniversalMaterial(shared_ptr<Material>(material));
			Shader* shader = new Shader("resources/simpleGeometry/");
			model->setModelUniversalShader(shared_ptr<Shader>(shader));
		}
		else if (actor == DefaultActor::grass)
		{
			PlantsLevel* plantsLevel = new PlantsLevel(level);

			string directory = "resources/3dplant/grass_wild_uddmcgbia/";
			vector<Actor*> temp;
			unsigned int varCount = 1;
			bool stillHaveVar = true;
			while (stillHaveVar)
			{
				string varName = "Var" + to_string(varCount);
				string varFolderPath = directory + varName + '/';
				struct stat buffer;
				if (stat(FileSystem::getPath(varFolderPath).c_str(), &buffer) == 0)
				{
					Actor* actor = new Actor(plantsLevel);
					QuixelModel* model = new QuixelModel(actor, varFolderPath, QuixelObjectType::_3dplant);
					QuixelMaterial* material = new QuixelMaterial(directory + "Textures/Billboard/", QuixelObjectType::_3dplant, Resolution::_2K);
					model->setModelUniversalMaterial(shared_ptr<Material>(material));
					Shader* shader = new Shader(directory.c_str());
					model->setModelUniversalShader(shared_ptr<Shader>(shader));

					temp.emplace_back(actor);
					varCount++;
				}
				else
				{
					stillHaveVar = false;
				}
			}
			plantsLevel->m_Plants.emplace_back(temp);
		}
	}
}
