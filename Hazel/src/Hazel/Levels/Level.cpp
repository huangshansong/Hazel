#include "hzpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


#include "Hazel/Application.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Actors/camera.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"

#include "Level.h"

using namespace std;
namespace Hazel
{
	bool Level::m_FirstLevel = true;

	void Level::init()
	{
        if (m_FirstLevel) {
            m_FirstLevel = false;

            Actor* camera = createCamera();
            m_Actors.emplace_back(camera);
            m_CurrentCamera = dynamic_cast<Camera*>(camera);

            if (Application::getCanDisplayTest())
            {
                Actor* actor = createActor("Backpack");
                m_Actors.emplace_back(actor);
            }
            else
            {
                Actor* actor;
                actor = createActor("Landscape");
                m_Actors.emplace_back(actor);
                actor = createActor("Sphere");
                m_Actors.emplace_back(actor);
            }
        }
	}

    void Level::onRender()
    {
        for (Actor* actor : m_Actors)
        {
            callActorOnRender(actor);
        }
    }

    void Level::onUpdate()
    {
        //should add physics processing here later!!!!!!!!!
        //multi-thread processing needed, physics one, and render one
        for (Actor* actor : m_Actors)
        {
            callActorOnUpdate(actor);
        }
        onRender();
    }

    void Level::onEvent(Event& event)
    {
        for (Actor* actor : m_Actors)
        {
            callActorOnEvent(actor, event);
        }
    }

    void Level::gen_ItemLocation()
    {
    }

}
