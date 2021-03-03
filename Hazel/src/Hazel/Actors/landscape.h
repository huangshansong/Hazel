#pragma once

#include "Hazel/ResourceLoading/Material.h"

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

	class Landscape
	{
	public:
		void gen_Landscape(const unsigned int width, const unsigned int length);

		inline int getWidth() const { return Width; }
		inline int getLength() const { return Length; }
		inline float getHeight(int x, int y) const { return heightMap[x][y]; }

	private:

		~Landscape();
		void mountains2heightMap(std::vector<Mountain>&);
		void setMainpeak(Peak&);
		void setPeak(Peak&, Peak&, Peak&);

		int Width;
		int Length;
		float Maxheight;
		float**  heightMap = nullptr;
		float** peak2hillMap = nullptr;
		
		std::vector<Material> materials;
	};
}