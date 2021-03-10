#pragma once
#include "hzpch.h"

#include "model.h"

namespace Hazel
{
	struct Peak
	{
		int x;
		int y;
		float height;
		float direction;
	};
	struct Mountain
	{
		std::vector<Peak> peaks;
	};

	//LandscapeModel is a model
	class LandscapeModel : public Model
	{
	public:
		class Interface
		{
			friend class Actor;
		private:
			static Model* create(void* actor, unsigned int width = 1000, unsigned int length = 1000) { return new LandscapeModel(actor);}
		};
		
		int getWidth() const { return m_Width; }
		int getLength() const { return m_Length; }
		float getHeight(int x, int y) const { return m_HeightMap[x][y]; }
		
	protected:
		LandscapeModel(void* actor, unsigned int width = 1000, unsigned int length = 1000);
		virtual ~LandscapeModel();

		void gen_Landscape();

		void mountains2heightMap(std::vector<Mountain>&);
		void setMainpeak(Peak&);
		void setPeak(Peak&, Peak&, Peak&);

		int m_Width;
		int m_Length;
		float m_Maxheight;
		float**  m_HeightMap = nullptr;
		float** m_Peak2hillMap = nullptr;

	};
}