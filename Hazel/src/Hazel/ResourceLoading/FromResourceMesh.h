#pragma once
#include "mesh.h"

namespace Hazel
{
	class FromResourceMesh : public Mesh
	{
	public:
		FromResourceMesh(void* model, std::vector<Vertex>* vertices = nullptr, std::vector<unsigned int>* indices = nullptr, const std::string name = "Unnamed");
	protected:
		virtual void drawAfterBindTextures() const override;
	};

}

