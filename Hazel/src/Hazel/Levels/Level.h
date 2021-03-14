#pragma once
#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Actors/camera.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Events/Event.h"
#include "Hazel/HObject.h"

namespace Hazel 
{
	class  HAZEL_API Level : public HObject
	{
	public:
		Level(Level* level, std::string = "Unnamed");
		Level(void* viewport, std::string = "Unnamed");

		virtual ~Level() = default;

		static void setCamera(Level* level, Camera* camera)
		{
			level->m_Camera = camera;
			for (std::shared_ptr<Level> childLevel : level->m_ChildLevels)
			{
				setCamera(childLevel.get(), camera);
			}
		}

		const Camera* getCamera() const{ return m_Camera; }

		const void* getOfViewport() const { return m_OfViewport; }

		virtual void onRender();

		virtual void onUpdate();

		virtual void onPlayerInputEvent(Event&);

		std::vector<std::shared_ptr<Level>> m_ChildLevels;

		std::vector<std::shared_ptr<Actor>> m_Actors;

		std::string m_Name;

	protected:
		void* m_OfViewport;
		Camera* m_Camera = nullptr;
		//manage levels with a tree structure
		
	};

}

