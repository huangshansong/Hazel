#pragma once
#include "hzpch.h"

#include "ProceduralModel.h"

namespace Hazel
{
	struct HAZEL_API Peak
	{
		int x;
		int y;
		float height;
		float direction;
	};
	struct HAZEL_API Mountain
	{
		std::vector<Peak> peaks;
	};

	//LandscapeModel is a model
	class HAZEL_API LandscapeModel : public ProceduralModel
	{
	public:
		LandscapeModel(void* actor, unsigned int width = 1000, unsigned int length = 1000);

		virtual ~LandscapeModel();

		int getWidth() { return m_Width; }

		int getLength() { return m_Length; }

		float getHeight(int x, int y) { return m_HeightMap[x][y]; }
		
	protected:
		virtual void setupModel() override;

		virtual void processMesh() override;

		void mountains2heightMap(std::vector<Mountain>&);

		void setMainpeak(Peak&);

		void setPeak(Peak&, Peak&, Peak&);

		int m_Width;
		int m_Length;
		float m_Maxheight;
		float**  m_HeightMap = nullptr;
		float** m_Peak2hillMap = nullptr;

	};

	class HAZEL_API LandscapeMesh : public Mesh
	{
	public:
		// initializes all the buffer objects/arrays
		LandscapeMesh(LandscapeModel* landscape);

		
	protected:
		virtual void setupMesh() override;

		virtual void drawAfterBindTextures() const override;

		unsigned int m_IndexCount = 0;

	};
}