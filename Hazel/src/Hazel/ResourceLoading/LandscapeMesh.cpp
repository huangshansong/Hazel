#include "hzpch.h"

#include "FileSystem.h"
#include "Material.h"
#include "LandscapeMesh.h"

using namespace std;

namespace Hazel
{
    LandscapeMesh::LandscapeMesh(LandscapeModel* landscape)
        : Mesh(landscape)
    {
        setupMesh(landscape);
        
        std::vector<std::string> folderPaths;
        folderPaths.emplace_back("resources/surfaces/grass_wild_sgmkajak/");
        folderPaths.emplace_back("resources/surfaces/snow_pure_uephfgudy/");
        setQuixelMaterial(folderPaths, getModelTexturesLoaded(), QuixelObjectType::_surface);

        bindShader();
    }
    void LandscapeMesh::setupMesh(LandscapeModel* landscape)
    {
        // data to fill, no need to store the data in m_XX
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        
        unsigned int Width = landscape->getWidth();
        unsigned int Length = landscape->getLength();

        for (unsigned int i = 0; i < Width - 1; i++)
        {
            for (unsigned int j = 0; j < Length - 1; j++)
            {
                Vertex vertex;

                glm::vec3 vectorLT;
                vectorLT.x = (float)i;
                vectorLT.z = (float)j;
                vectorLT.y = landscape->getHeight(i, j);

                glm::vec3 vectorRT;
                vectorRT.x = (float)i;
                vectorRT.z = (float)(j + 1);
                vectorRT.y = landscape->getHeight(i, j + 1);

                glm::vec3 vectorLB;
                vectorLB.x = (float)(i + 1);
                vectorLB.z = (float)j;
                vectorLB.y = landscape->getHeight(i + 1, j);

                glm::vec3 vectorRB;
                vectorRB.x = (float)(i + 1);
                vectorRB.z = (float)(j + 1);
                vectorRB.y = landscape->getHeight(i + 1, j + 1);

                glm::vec3 normalofLTtriangel = glm::normalize(glm::cross(vectorLT - vectorRT, vectorLB - vectorLT));
                glm::vec3 normalofRBtriangel = glm::normalize(glm::cross(vectorRB - vectorLB, vectorRT - vectorRB));

                vertex.Position = vectorLT / 10.0f;
                vertex.Normal = normalofLTtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                vertices.emplace_back(vertex);
                vertex.Position = vectorRT / 10.0f;
                vertex.Normal = normalofLTtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                vertices.emplace_back(vertex);
                vertex.Position = vectorLB / 10.0f;
                vertex.Normal = normalofLTtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                vertices.emplace_back(vertex);
                vertex.Position = vectorLB / 10.0f;
                vertex.Normal = normalofRBtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                vertices.emplace_back(vertex);
                vertex.Position = vectorRT / 10.0f;
                vertex.Normal = normalofRBtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                vertices.emplace_back(vertex);
                vertex.Position = vectorRB / 10.0f;
                vertex.Normal = normalofRBtriangel;
                vertex.TexCoords = glm::vec2(vertex.Position.x / 10.0f, vertex.Position.z / 10.0f);
                //vertex.TexCoords = glm::vec2(0.5, 0.5);
                vertices.emplace_back(vertex);

            }
        }

        
        m_IndexCount = Width * Length * 6;

        bindBufferAndAttribute(vertices, indices);

    }

    void LandscapeMesh::drawAfterBindTextures() const
    {
        // draw mesh
        glBindVertexArray(m_VAO);
        //HZ_CORE_INFO("glBindVertexArray(m_VAO);");

        glDrawArrays(GL_TRIANGLES, 0, m_IndexCount);


        glBindVertexArray(0);
    }

}
