#pragma once
#include "hzpch.h"

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
		~Landscape();

		void gen_Landscape(const unsigned int width, const unsigned int length);

		inline int getWidth() const { return m_Width; }
		inline int getLength() const { return m_Length; }
		inline float getHeight(int x, int y) const { return m_HeightMap[x][y]; }
		inline std::vector<std::string>& getMaterialsPaths() const { return (std::vector<std::string>&)m_MaterialsPaths; }

	private:

		void mountains2heightMap(std::vector<Mountain>&);
		void setMainpeak(Peak&);
		void setPeak(Peak&, Peak&, Peak&);

		int m_Width;
		int m_Length;
		float m_Maxheight;
		float**  m_HeightMap = nullptr;
		float** m_Peak2hillMap = nullptr;
		
		std::vector<std::string> m_MaterialsPaths;
	};
}