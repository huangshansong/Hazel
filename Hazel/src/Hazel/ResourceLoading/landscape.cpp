#include "hzpch.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/vector_angle.hpp>
#include <glm/glm/geometric.hpp>

#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "Hazel/HazelMathLib.h"

#include "landscape.h"

using namespace std;
namespace Hazel
{
	LandscapeModel::LandscapeModel(void* actor, unsigned int width, unsigned int length)
		: ProceduralModel(actor), m_Width(width), m_Length(length)
	{
		setupModel();

		processMesh();
	}

	LandscapeModel::~LandscapeModel()
	{
		if (m_HeightMap != nullptr)
		{
			for (int i = 1; i < m_Width; i++)
			{
				if (m_HeightMap[i] != nullptr)
				{
					free(m_HeightMap[i]);
				}
			}
			free(m_HeightMap);
		}
		if (m_Peak2hillMap != nullptr)
		{
			for (int i = 1; i < m_Width; i++)
			{
				if (m_Peak2hillMap[i] != nullptr)
				{
					free(m_Peak2hillMap[i]);
				}
			}
			free(m_Peak2hillMap);
		}
	}

	void LandscapeModel::setupModel()
	{
		//set the MAXHEIGHT according to the WIDTH and LENGTH
		m_Maxheight = std::fminf(m_Width, m_Length) / 4.0f;

		//malloc and set peak2hillMap only once
		if (m_Peak2hillMap == nullptr)
		{
			while (m_Peak2hillMap == nullptr)
			{
				m_Peak2hillMap = (float**)malloc(sizeof(float*) * m_Width);

				if(m_Peak2hillMap == nullptr) HZ_CORE_INFO("peak2hillMap not successfully malloced!");
			}
			for (int i = 0; i < m_Width; i++)
			{
				m_Peak2hillMap[i] = nullptr;
				while (m_Peak2hillMap[i] == nullptr)
				{
					m_Peak2hillMap[i] = (float*)malloc(sizeof(float) * m_Length);

					if (m_Peak2hillMap[i] == nullptr) HZ_CORE_INFO("peak2hillMap not successfully malloced!");
				}
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

		//malloc heightMap
		if (m_HeightMap != nullptr)
		{
			for (int i = 1; i < m_Width; i++)
			{
				if (m_HeightMap[i] != nullptr)
				{
					free(m_HeightMap[i]);
				}
			}
			free(m_HeightMap);
		}
		m_HeightMap = nullptr;
		while (m_HeightMap == nullptr)
		{
			m_HeightMap = (float**)malloc(sizeof(float*) * m_Width);

			if(m_HeightMap == nullptr) HZ_CORE_INFO("heightMap not successfully malloced!");
		}
		for (int i = 0; i < m_Width; i++)
		{
			m_HeightMap[i] = nullptr;
			while (m_HeightMap[i] == nullptr)
			{
				m_HeightMap[i] = (float*)malloc(sizeof(float) * m_Length);

				if (m_HeightMap[i] == nullptr) HZ_CORE_INFO("heightMap[i] not successfully malloced!");
			}
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
	void LandscapeModel::processMesh()
	{
		vector<shared_ptr<Mesh>> temp;
		m_LODs.emplace_back(temp);
		m_LODs[0].emplace_back(shared_ptr<Mesh>(new LandscapeMesh(this)));
	}

	void LandscapeModel::mountains2heightMap(std::vector<Mountain>& mountains)
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

	void LandscapeModel::setMainpeak(Peak& mainpeak)
	{
		std::random_device rd;
		std::default_random_engine random_Generator{ rd() };
		std::normal_distribution<float> distribution;

		//the first parameter is the mean(ж╠), the second is the standard deviation(ж╥) 
		//probability of plus or minus 1 standard deviation(ж╥) = 68.3%
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

	void LandscapeModel::setPeak(Peak& peakCur, Peak& peakPre, Peak& mainpeak)
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

	LandscapeMesh::LandscapeMesh(LandscapeModel* landscape)
		: ProceduralMesh(landscape)
	{
		setupMesh();
		bindBufferAndAttribute();
	}

	void LandscapeMesh::drawAfterBindTextures() const
	{
		// draw mesh
		glBindVertexArray(m_VAO);
		//HZ_CORE_INFO("glBindVertexArray(m_VAO);");

		glDrawElements(GL_TRIANGLE_STRIP, m_Indices->size(), GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);
	}

	void LandscapeMesh::setupMesh()
	{
		LandscapeModel* landscapeModel = (LandscapeModel*)m_OfModel;
		unsigned int WIDTH = landscapeModel->getWidth();
		unsigned int LENGTH = landscapeModel->getLength();

		m_Vertices = new vector<Vertex>;
		m_Indices = new vector<unsigned int>;
		for (unsigned int i = 0; i < WIDTH; i++)
		{
			for (unsigned int j = 0; j < LENGTH; j++)
			{
				Vertex vertexTemp;
				vertexTemp.Position = glm::vec3(i, landscapeModel->getHeight(i, j), j) / 10.0f;// 10.0f is just about scale
				vertexTemp.Normal = glm::vec3(0.0f);//set normal to (0,0,0)
				m_Vertices->emplace_back(vertexTemp);
			}
		}
		//calculate the normal
		for (unsigned int i = 0; i < WIDTH - 1; i++)
		{
			for (unsigned int j = 0; j < LENGTH - 1; j++)
			{
				glm::vec3 normalofLTtriangel;//left top triangle
				glm::vec3 normalofRBtriangel;// right bottom triangle
				glm::vec3 up = (*m_Vertices)[i * LENGTH + j].Position - (*m_Vertices)[i * LENGTH + j + 1].Position;
				glm::vec3 left = (*m_Vertices)[(i + 1) * LENGTH + j].Position - (*m_Vertices)[i * LENGTH + j].Position;
				normalofLTtriangel = glm::normalize(glm::cross(up, left));
				glm::vec3 down = (*m_Vertices)[(i + 1) * LENGTH + j + 1].Position - (*m_Vertices)[(i + 1) * LENGTH + j].Position;
				glm::vec3 right = (*m_Vertices)[i * LENGTH + j + 1].Position - (*m_Vertices)[(i + 1) * LENGTH + j + 1].Position;
				normalofRBtriangel = glm::normalize(glm::cross(down,right));
				(*m_Vertices)[i * LENGTH + j].Normal += normalofLTtriangel / 4.0f;// 1/4 of 360бу
				(*m_Vertices)[(i + 1) * LENGTH + j].Normal += normalofLTtriangel / 8.0f;// 1/8 of 360бу
				(*m_Vertices)[i * LENGTH + j + 1].Normal += normalofLTtriangel / 8.0f;// 1/8 of 360бу
				(*m_Vertices)[(i + 1) * LENGTH + j + 1].Normal += normalofRBtriangel / 4.0f;// 1/4 of 360бу
				(*m_Vertices)[(i + 1) * LENGTH + j].Normal += normalofRBtriangel / 8.0f;// 1/8 of 360бу
				(*m_Vertices)[i * LENGTH + j + 1].Normal += normalofRBtriangel / 8.0f;// 1/8 of 360бу		
			}
		}
		for (unsigned int i = 0; i < WIDTH; i++)
		{
			for (unsigned int j = 0; j < LENGTH; j++)
			{
				(*m_Vertices)[i * LENGTH + j].Normal = glm::normalize((*m_Vertices)[i * LENGTH + j].Normal);
			}
		}
		//calculate the texCoords
		for (unsigned int i = 0; i < WIDTH - 1; i++)
		{
			if (i == 0)
			{
				//only the first two points' texCoords are set by myself, others' are calculated to match the triangle's shape in the world space and the triangle's shape in texture
				(*m_Vertices)[0].TexCoords = glm::vec2(0.0f, 0.0f);
				(*m_Vertices)[1].TexCoords = glm::vec2(0.0f, glm::distance((*m_Vertices)[0].Position, (*m_Vertices)[1].Position));//you can scale the texture up by dividing this
				calcAndSetTexCoords(1 * LENGTH, 1, 0);
				calcAndSetTexCoords(1 * LENGTH + 1, 1, 1 * LENGTH);
				for (unsigned int j = 2; j < LENGTH; j++)
				{
					calcAndSetTexCoords(j, j - 1, 1 * LENGTH + j - 1);
					calcAndSetTexCoords(1 * LENGTH + j, j, 1 * LENGTH + j - 1);
				}
			}
			else
			{
				calcAndSetTexCoords((i + 1) * LENGTH, i * LENGTH + 1, i * LENGTH);
				calcAndSetTexCoords((i + 1) * LENGTH + 1, i * LENGTH + 1, (i + 1) * LENGTH);
				for (unsigned int j = 1; j < LENGTH - 1; j++)
				{
					calcAndSetTexCoords((i + 1) * LENGTH + j, i * LENGTH + j + 1, i * LENGTH + j, true);
					calcAndSetTexCoords((i + 1) * LENGTH + j + 1, i * LENGTH + j + 1, (i + 1) * LENGTH + j);
				}
			}
		}
		//the draw sequence is zig-zag
		bool oddRow = false;
        for (unsigned int i = 0; i < WIDTH - 1; i++)
        {
            if (!oddRow) // even rows: i == 0, i == 2; and so on
            {
                for (unsigned int j = 0; j < LENGTH; j++)
                {
                    m_Indices->emplace_back(i * LENGTH + j);
                    m_Indices->emplace_back((i + 1) * LENGTH + j);
                }
            }
            else
            {
                for (int j = LENGTH - 1; j >= 0; j--)
                {
                    m_Indices->emplace_back((i + 1) * LENGTH + j);
                    m_Indices->emplace_back(i * LENGTH + j);
                }
            }
            oddRow = !oddRow;
        }

	}
	//when put the parameters, make sure the rotation angle is positive
	void LandscapeMesh::calcAndSetTexCoords(unsigned int targetVertexIndex, int preparedVertex1Index, int preparedVertex2Index, bool interpolation)
	{
		glm::vec3 P1toT_Position = (*m_Vertices)[targetVertexIndex].Position - (*m_Vertices)[preparedVertex1Index].Position;
		glm::vec2 P1toP2_TexCoords = (*m_Vertices)[preparedVertex2Index].TexCoords - (*m_Vertices)[preparedVertex1Index].TexCoords;
		glm::vec3 P1toP2_Position = (*m_Vertices)[preparedVertex2Index].Position - (*m_Vertices)[preparedVertex1Index].Position;
		float angle = glm::angle(glm::normalize(P1toT_Position), glm::normalize(P1toP2_Position));
		glm::vec2 P1toT_TexCoords = glm::normalize(glm::rotate(P1toP2_TexCoords, angle));

		if (interpolation)
		{
			(*m_Vertices)[targetVertexIndex].TexCoords = ((*m_Vertices)[targetVertexIndex].TexCoords + 
				(*m_Vertices)[preparedVertex1Index].TexCoords + glm::length(P1toT_Position) * P1toT_TexCoords) / 2.0f;
		}
		else
		{
			(*m_Vertices)[targetVertexIndex].TexCoords = (*m_Vertices)[preparedVertex1Index].TexCoords + glm::length(P1toT_Position) * P1toT_TexCoords;
		}
	}
}
	  