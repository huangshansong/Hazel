#pragma once

#include "Geometry.h"
#include "ProceduralModel.h"
#include "ProceduralMesh.h"

namespace Hazel
{

    class HAZEL_API SphereModel : public ProceduralModel
    {
    public:
        SphereModel(void*, Sphere = Sphere());

		const Sphere& getSphere() const { return m_Sphere; }

	protected:
		virtual void setupModel() override {};

		virtual void processMesh() override;

		Sphere m_Sphere;
    };

	class HAZEL_API SphereMesh : public ProceduralMesh
	{
	public:
		SphereMesh(void*, const std::string name = "Unnamed");

	protected:
		// initializes all the buffer objects/arrays
		virtual void setupMesh() override;

		virtual void drawAfterBindTextures() const override;
	};
}


