#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "Hazel/HazelMathLib.h"

#include "landscape.h"

namespace Hazel
{

	void Landscape::gen_Landscape(const unsigned int width, const unsigned int length)
	{
		m_Width = width;
		m_Length = length;
		//set the MAXHEIGHT according to the WIDTH and LENGTH
		m_Maxheight = std::fminf(m_Width, m_Length) / 4.0f;

		//malloc heightMap
		if (m_HeightMap != nullptr)
		{
			for (int i = 1; i < m_Width; i++)
			{
				free(m_HeightMap[i]);
			}
			free(m_HeightMap);
		}
		m_HeightMap = nullptr;
		m_HeightMap = (float**)malloc(sizeof(float*) * m_Width);
		HZ_CORE_ASSERT(m_HeightMap, "heightMap not successfully malloced!");
		for (int i = 0; i < m_Width; i++)
		{
			m_HeightMap[i] = (float*)malloc(sizeof(float) * m_Length);
			HZ_CORE_ASSERT(m_HeightMap[i], "heightMap not successfully malloced!");
			//HZ_CORE_INFO((void*)heightMap[i]);
		}
		//set heightMap values
		for (int j = 0; j < m_Length; j++)
		{
			for (int i = 1; i < m_Width; i++)
			{
				m_HeightMap[i][j] = 1E-5f;
				//HZ_CORE_INFO(heightMap[i][j]);
			}
		}
		//malloc peak2hillMap
		if (m_Peak2hillMap == nullptr)
		{
			m_Peak2hillMap = (float**)malloc(sizeof(float*) * m_Width);
			HZ_CORE_ASSERT(m_Peak2hillMap, "peak2hillMap not successfully malloced!");
			for (int i = 0; i < m_Width; i++)
			{
				m_Peak2hillMap[i] = nullptr;
				m_Peak2hillMap[i] = (float*)malloc(sizeof(float) * m_Length);
				HZ_CORE_ASSERT(m_Peak2hillMap[i], "peak2hillMap not successfully malloced!");
			}
			//set peak2hillMap values
			for (int j = 0; j < m_Length; j++)
			{
				m_Peak2hillMap[0][j] = normalDistributionFunc((float)j, 0.0f, std::fminf(m_Width, m_Length) / 12);//the value 12 determine the shape of the hill
				for (int i = 1; i < m_Width; i++)
				{
					m_Peak2hillMap[i][j] = m_Peak2hillMap[0][j] / m_Peak2hillMap[0][0] * normalDistributionFunc((float)i, 0.0f, std::fminf(m_Width, m_Length) / 12);
				}
			}
			float peakPoint = normalDistributionFunc(0.0f, 0.0f, std::fminf(m_Width, m_Length) / 12);
			for (int i = 0; i < m_Width; i++)
			{
				for (int j = 0; j < m_Length; j++)
				{
					m_Peak2hillMap[i][j] /= peakPoint;
					//HZ_CORE_INFO(peak2hillMap[i][j]);
				}
			}
			//HZ_CORE_INFO(peak2hillMap[0][99]);
			//HZ_CORE_INFO(peak2hillMap[99][0]);
			//heightMap = peak2hillMap;
			//return;
		}

		//set the number of total peaks
		std::random_device rd;
		std::default_random_engine random_Generator{ rd() };
		std::normal_distribution<float> distribution;
		int peaksTotal = -1;
		distribution.param(std::normal_distribution<float>::param_type(75, 25));
		while (peaksTotal < 50 || peaksTotal >= 100)
		{
			peaksTotal = (int)distribution(random_Generator);
		}
		//generate all mountains
		std::vector<Mountain> mountains;
		int peakCount = 0;
		while (peakCount < peaksTotal)
		{
			Mountain mountain;
			
			int peaksOfMountain = -1;
			distribution.param(std::normal_distribution<float>::param_type(10, 10));
			while (peaksOfMountain < 0 || peaksOfMountain >= 20)
			{
				peaksOfMountain = (int)distribution(random_Generator);
			}
			
			Peak mainpeak;
			setMainpeak(mainpeak);
			mountain.peaks.emplace_back(mainpeak);
			peakCount++;

			Peak peakPre = mainpeak;
			for (int i = 2; i <= peaksOfMountain / 2; i++)
			{
				Peak peakCur;
				setPeak(peakCur, peakPre, mainpeak);
				if (0 <= peakCur.x && peakCur.x < m_Width && 0 <= peakCur.y && peakCur.y < m_Length)
				{
					mountain.peaks.emplace_back(peakCur);
					peakCount++;
					peakPre = peakCur;
				}
				else
				{
					break;
				}
			}
			peakPre = mainpeak;
			peakPre.direction = 2 * std::_Pi - mainpeak.direction;
			for (int i = peaksOfMountain / 2 + 1; i <= peaksOfMountain; i++)
			{
				Peak peakCur;
				setPeak(peakCur, peakPre, mainpeak);
				if (0 <= peakCur.x && peakCur.x < m_Width && 0 <= peakCur.y && peakCur.y < m_Length)
				{
					mountain.peaks.emplace_back(peakCur);
					peakCount++;
					peakPre = peakCur;
				}
				else
				{
					break;
				}
			}

			mountains.emplace_back(mountain);
		}

		mountains2heightMap(mountains);
	}

	Landscape::~Landscape()
	{
		if (m_HeightMap != nullptr)
		{
			for (int i = 1; i < m_Width; i++)
			{
				free(m_HeightMap[i]);
			}
			free(m_HeightMap);
		}
		if (m_Peak2hillMap != nullptr)
		{
			for (int i = 1; i < m_Width; i++)
			{
				free(m_Peak2hillMap[i]);
			}
			free(m_Peak2hillMap);
		}
		
	}

	void Landscape::mountains2heightMap(std::vector<Mountain>& mountains)
	{
		for (Mountain mountain : mountains)
		{
			for (Peak peak : mountain.peaks)
			{
				for (int i = 0; i < m_Width; i++)
				{
					for (int j = 0; j < m_Length; j++)
					{
						
						m_HeightMap[i][j] = heightInteractionFunc(m_Peak2hillMap[std::abs(i - peak.x)][std::abs(j - peak.y)] * peak.height, m_HeightMap[i][j]);
						//if(heightMap[i][j] > 1E2f || heightMap[i][j] < 0.0f) HZ_CORE_INFO(heightMap[i][j]);
					}
				}
			}
		}

		
	}

	void Landscape::setMainpeak(Peak& mainpeak)
	{
		std::random_device rd;
		std::default_random_engine random_Generator{ rd() };
		std::normal_distribution<float> distribution;

		//the first parameter is the mean(¦Ì), the second is the standard deviation(¦Ò) 
		//probability of plus or minus 1 standard deviation(¦Ò) = 68.3%
		distribution.param(std::normal_distribution<float>::param_type(m_Width / 2, m_Width / 2));
		mainpeak.x = -1;
		distribution.param(std::normal_distribution<float>::param_type(m_Length / 2, m_Length / 2));
		mainpeak.y = -1;
		int count = 0;
		do
		{
			while (mainpeak.x < 0 || mainpeak.x >= m_Width)
			{
				mainpeak.x = (int)distribution(random_Generator);
			}
			while (mainpeak.y < 0 || mainpeak.y >= m_Length)
			{
				mainpeak.y = (int)distribution(random_Generator);
			}
			count++;

		} while (count <= 100 || m_HeightMap[mainpeak.x][mainpeak.y] > m_Maxheight / 1E1f);
		

		distribution.param(std::normal_distribution<float>::param_type(m_Maxheight / 2, m_Maxheight / 2));
		mainpeak.height = -1.0f;
		while (mainpeak.height < 1.0f || mainpeak.height > m_Maxheight)
		{
			mainpeak.height = distribution(random_Generator);
		}

		distribution.param(std::normal_distribution<float>::param_type(std::_Pi, 1E5f));
		mainpeak.direction = -1.0f;
		while (mainpeak.direction < 1E-5f || mainpeak.direction > 2 * std::_Pi)
		{
			mainpeak.direction = distribution(random_Generator);
		}

	}

	void Landscape::setPeak(Peak& peakCur, Peak& peakPre, Peak& mainpeak)
	{
		std::random_device rd;
		std::default_random_engine random_Generator{ rd() };
		std::normal_distribution<float> distribution;

		distribution.param(std::normal_distribution<float>::param_type(peakPre.direction, std::_Pi / 16.0f));
		peakCur.direction = -10.0f;
		while (std::abs(peakCur.direction - peakPre.direction) > std::_Pi / 4.0f)
		{
			peakCur.direction = distribution(random_Generator);
		}
		distribution.param(std::normal_distribution<float>::param_type(peakPre.height * 4.0f / 5.0f, peakPre.height * 4.0f / 5.0f / 2.0f));//4.0f is the length/height ratio.
		float distance = -1.0f;
		while (distance < 1E-5f)
		{
			distance = distribution(random_Generator);
		}
		peakCur.x = peakPre.x + (int)(distance * std::cosf(peakCur.direction));
		peakCur.y = peakPre.y + (int)(distance * std::sinf(peakCur.direction));
		distribution.param(std::normal_distribution<float>::param_type(peakPre.height - distance / 4.0f, distance / 4.0f));//4.0f is the length/height ratio.
		peakCur.height = -1.0f;
		while (peakCur.height < 1E-5f || peakCur.height > mainpeak.height)
		{
			peakCur.height = distribution(random_Generator);
		}
	}
}
	  