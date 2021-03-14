#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/HObject.h"
#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ResourceLoading/model.h"


namespace Hazel 
{
	//add every new default actor's name here
	const char* const defaultActorNames[] =
	{
		"Backpack",
		"Landscape",
		"Sphere",
		"Grass"
	};
	enum class DefaultActor
	{
		backpack = 0,
		landscape,
		sphere,
		grass
	};

	class HAZEL_API Actor : public HObject
	{
	public:
		Actor(void* level, std::string name = "Unnamed", glm::vec3 transform = glm::vec3(0.0f, 0.0f, 0.0f));

		virtual ~Actor() = default;

		void setOfLevel(void* level) { m_OfLevel = level; }

		void setName(std::string& name) { m_Name = name; }
		
		void setTransform(glm::vec3& transform) { m_Transform = transform; }
		
		const void* getOfLevel() const { return m_OfLevel; }
		
		const std::string& getName() const { return m_Name; }

		const glm::vec3& getTransform() const { return m_Transform; }
		
		virtual void onPlayerInputEvent(Event&);

		virtual void onUpdate();

		virtual void onRender();

		std::shared_ptr<Model> m_Model = nullptr;

		std::string m_Name = "Unnamed";

		glm::vec3 m_Transform;

	protected:
		void* m_OfLevel;
		
	};
}