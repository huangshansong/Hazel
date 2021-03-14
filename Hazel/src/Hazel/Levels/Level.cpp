#include "hzpch.h"

#include "Hazel/Application.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Actors/camera.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"

#include "Level.h"

using namespace std;
namespace Hazel
{
    Level::Level(Level* level, std::string name)
        : m_OfViewport(level->m_OfViewport), m_Name(name)
    {
        level->m_ChildLevels.emplace_back(shared_ptr<Level>(this));
        setCamera(this, level->m_Camera);
    }
    Level::Level(void* viewport, std::string name)
        : m_OfViewport(viewport), m_Name(name)
    {
        ((Viewport*)viewport)->m_RootLevels.emplace_back(shared_ptr<Level>(this));
    }
    void Level::onRender()
    {
        for (shared_ptr<Actor> actor : m_Actors)
        {
            actor->onRender();
        }
        for (shared_ptr<Level> childLevel : m_ChildLevels)
        {
            childLevel->onRender();
        }
    }

    void Level::onUpdate()
    {
        //should add physics processing here later!!!!!!!!!
        for (shared_ptr<Actor> actor : m_Actors)
        {
            actor->onUpdate();
        }
        for (shared_ptr<Level> childLevel : m_ChildLevels)
        {
            childLevel->onUpdate();
        }
        
    }

    void Level::onPlayerInputEvent(Event& event)
    {
        for (shared_ptr<Actor> actor : m_Actors)
        {
            actor->onPlayerInputEvent(event);
        }
        for (shared_ptr<Level> childLevel : m_ChildLevels)
        {
            childLevel->onPlayerInputEvent(event);
        }

    }

}
