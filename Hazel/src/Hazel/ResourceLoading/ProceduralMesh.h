#pragma once

#include "mesh.h"

namespace Hazel
{
	class ProceduralMesh : public Mesh
	{
	public:
		ProceduralMesh(void* model, const std::string name = "Unnamed");

	protected:
		virtual void setupMesh() = 0;

	};
}


