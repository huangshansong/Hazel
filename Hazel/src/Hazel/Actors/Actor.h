#pragma once
#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ResourceLoading/model.h"

namespace Hazel 
{
	class HAZEL_API Actor
	{
		friend class LevelLayer;
		friend class Level;
	public:
		inline Model* getModel() const { return m_Model; }

	private:
		virtual void onEvent(Event&);
		virtual void onUpdate();
		virtual void onRender();

		inline void setModel(Model* model) { m_Model = model; }

		Model* m_Model;
	};
}