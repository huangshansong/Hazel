#pragma once
#include "hzpch.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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
		"Grass",
		"Enviroment",
		"Cerberus",
		"Container"
	};
	enum class DefaultActor
	{
		backpack = 0,
		landscape,
		sphere,
		grass,
		enviroment,
		cerberus,
		container
	};

	class HAZEL_API Actor : public HObject
	{
	public:
		Actor(void* level, std::string name = "Unnamed");

		virtual ~Actor() = default;

		void setOfLevel(void* level) { m_OfLevel = level; }

		void setName(std::string& name) { m_Name = name; }
		
		void setTransform(glm::vec3& transform) { m_Transform = transform; }
		
		void setRotation(glm::mat3& rotation) { m_Rotation = rotation; }

		const void* getOfLevel() const { return m_OfLevel; }
		
		const std::string& getName() const { return m_Name; }

		const glm::vec3& getTransform() const { return m_Transform; }
		
		const glm::mat3& getRotation() const { return m_Rotation; }

		virtual void onPlayerInputEvent(Event&);

		virtual void onUpdate();

		virtual void onRender() const;

		std::shared_ptr<Model> m_Model = nullptr;

		std::string m_Name = "Unnamed";

		//The default is to look at the positive z axis from the origin of the coordinate
		glm::vec3 m_Transform = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat3 m_Rotation = glm::mat3(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	protected:
		void* m_OfLevel;
		
	};
}