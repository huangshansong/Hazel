#include "hzpch.h"

#include "FileSystem.h"
#include "SimpleGeometryMesh.h"

using namespace std;

namespace Hazel
{
    SphereMesh::SphereMesh(void* model)
        : Mesh(model)
    {
        setupMesh();

        std::vector<std::string> folderPaths;
        folderPaths.emplace_back("resources/surfaces/ground_other_vbljegefw/");
        setQuixelMaterial(folderPaths, getModelTexturesLoaded(), QuixelObjectType::_surface);

        bindShader();
    }
    void SphereMesh::setupMesh()
    {
        //sphere's vertices are useless, so no need to push these in the m_Vertices and m_Indices
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                Vertex vertex;
                vertex.Position = glm::vec3(xPos, yPos, zPos);
                vertex.Normal = glm::vec3(xPos, yPos, zPos);
                vertex.TexCoords = glm::vec2(xSegment, ySegment);
                vertices.emplace_back(vertex);
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        m_IndexCount = indices.size();

        bindBufferAndAttribute(vertices, indices);
    }

    void SphereMesh::drawAfterBindTextures() const
    {
        // draw mesh
        glBindVertexArray(m_VAO);
        //HZ_CORE_INFO("glBindVertexArray(m_VAO);");

        glDrawElements(GL_TRIANGLE_STRIP, m_IndexCount, GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);
    }

}
