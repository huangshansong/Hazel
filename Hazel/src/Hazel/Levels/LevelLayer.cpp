#include "hzpch.h"

#include "Hazel/Actors/Actor.h"
#include "Hazel/Application.h"

#include "LevelLayer.h"
#include "Level.h"

namespace Hazel
{
    
    void LevelLayer::OnEvent(Event& event)
    {
        for (Actor* actor : Application::m_Window->m_Viewport->currentLevel->m_Actors)
        {
            actor->OnEvent(event);
        }
    }
    
    void LevelLayer::OnUpdate()
    {
        for (Actor* actor : Application::m_Window->m_Viewport->currentLevel->m_Actors)
        {
            actor->OnUpdate();
        }
        OnRender();
    }

    void LevelLayer::OnAttach()
    {
        Application::m_Window->m_Viewport->currentLevel = new Level();
        Application::m_Window->m_Viewport->currentLevel->Init();
    }

    void LevelLayer::OnDetach()
    {
        delete Application::m_Window->m_Viewport->currentLevel;
    }

    void LevelLayer::OnRender()
    {
        Application::m_Window->m_Viewport->currentLevel->OnRender();
    }
}

