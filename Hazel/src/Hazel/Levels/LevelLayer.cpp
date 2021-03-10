#include "hzpch.h"

#include "Hazel/Actors/Actor.h"
#include "Hazel/Application.h"
#include "Hazel/Core.h"
#include "Hazel/Log.h"

#include "Hazel/ViewportLayer.h"
#include "LevelLayer.h"
#include "Level.h"

namespace Hazel
{
    
    void LevelLayer::onEvent(Event& event)
    {
        Application::getWindow()->getViewport()->getLevel()->onEvent(event);
    }
    
    void LevelLayer::onUpdate()
    {
        HZ_CORE_ASSERT(Application::getWindow()->getViewport()->getLevel(), 
            "NO CURRENTLEVEL! CHECK IF THERE IS NO LEVEL IN THE STACK!");

        Application::getWindow()->getViewport()->getLevel()->onUpdate(); 
    }

    void LevelLayer::onAttach()
    {
        Level* level = new Level;
        ViewportLayer::setLevel(Application::getWindow()->getViewport(), level);
        level->init();
    }

    void LevelLayer::onDetach()
    {
        delete Application::getWindow()->getViewport()->getLevel();
    }

    void LevelLayer::onRender()
    {
    }
}

