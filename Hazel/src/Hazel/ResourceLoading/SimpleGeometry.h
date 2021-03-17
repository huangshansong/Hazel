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
	class HAZEL_API CubeModel : public ProceduralModel
	{
	public:
		CubeModel(void*, Cube = Cube());

		const Cube& getCube() const { return m_Cube; }

	protected:
		virtual void setupModel() override {};

		virtual void processMesh() override;

		Cube m_Cube;
	};
	class HAZEL_API QuadModel : public ProceduralModel
	{
	public:
		QuadModel(void*, Quad = Quad());

		const Quad& getCube() const { return m_Quad; }

	protected:
		virtual void setupModel() override {};

		virtual void processMesh() override;

		Quad m_Quad;
	};
	class HAZEL_API SphereMesh : public ProceduralMesh
	{
	public:
		SphereMesh(void*, const std::string name = "Unnamed");

		virtual void drawAfterBindTextures() const override;

	protected:
		// initializes all the buffer objects/arrays
		virtual void setupMesh() override;
	
	};

	class HAZEL_API CubeMesh : public ProceduralMesh
	{
	public:
		CubeMesh(void*, const std::string name = "Unnamed");

		virtual void drawAfterBindTextures() const override;

	protected:
		// initializes all the buffer objects/arrays
		virtual void setupMesh() override;
	
	};
	class HAZEL_API QuadMesh : public ProceduralMesh
	{
	public:
		QuadMesh(void*, const std::string name = "Unnamed");

		virtual void drawAfterBindTextures() const override;

	protected:
		// initializes all the buffer objects/arrays
		virtual void setupMesh() override;

	};
}


