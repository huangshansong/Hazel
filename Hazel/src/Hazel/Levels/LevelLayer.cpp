#include "hzpch.h"

#include "Hazel/Actors/Actor.h"
#include "Hazel/Application.h"
#include "Hazel/Core.h"
#include "Hazel/Log.h"

#include "LevelLayer.h"
#include "Level.h"

namespace Hazel
{
    
    void LevelLayer::onEvent(Event& event)
    {
        for (Actor* actor : Application::getWindow()->getViewport()->getLevel()->m_Actors)
        {
            actor->onEvent(event);
        }
    }
    
    void LevelLayer::onUpdate()
    {
        HZ_CORE_ASSERT(Application::getWindow()->getViewport()->getLevel(), "NO CURRENTLEVEL! CHECK IF THERE IS NO LEVEL IN THE STACK!");

        onRender();

        //levelLayer manage the actorLayer, 
        //instead of calling actorLayer->OnUpdate() in the Application.cpp
        for (Actor* actor : Application::getWindow()->getViewport()->getLevel()->m_Actors)
        {
            actor->onUpdate();
        }
        
    }

    void LevelLayer::onAttach()
    {
        Level* level = new Level;
        level->init();
        Viewport::LevelSetter::setLevel(Application::getWindow()->getViewport(), level);
    }

    void LevelLayer::onDetach()
    {
        delete Application::getWindow()->getViewport()->getLevel();
    }

    void LevelLayer::onRender()
    {
        Application::getWindow()->getViewport()->getLevel()->onRender();
    }
}

