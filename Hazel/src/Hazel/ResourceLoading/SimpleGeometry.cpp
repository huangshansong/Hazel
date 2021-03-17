#include "hzpch.h"

#include "SimpleGeometry.h"

using namespace std;
namespace Hazel
{
	SphereModel::SphereModel(void* actor, Sphere sphere)
		: ProceduralModel(actor), m_Sphere(sphere)
	{
        setupModel();

        processMesh();
	}

    void SphereModel::processMesh()
    {
        vector<shared_ptr<Mesh>> temp;
        m_LODs.emplace_back(temp);
        m_LODs[0].emplace_back(shared_ptr<Mesh>(new SphereMesh(this)));
    }
    CubeModel::CubeModel(void* actor, Cube cube)
        : ProceduralModel(actor), m_Cube(cube)
    {
        setupModel();

        processMesh();
    }
    void CubeModel::processMesh()
    {
        vector<shared_ptr<Mesh>> temp;
        m_LODs.emplace_back(temp);
        m_LODs[0].emplace_back(shared_ptr<Mesh>(new CubeMesh(this)));
    }
    QuadModel::QuadModel(void* actor, Quad quad)
        : ProceduralModel(actor), m_Quad(quad)
    {
        setupModel();

        processMesh();
    }

    void QuadModel::processMesh()
    {
        vector<shared_ptr<Mesh>> temp;
        m_LODs.emplace_back(temp);
        m_LODs[0].emplace_back(shared_ptr<Mesh>(new QuadMesh(this)));
    }

    SphereMesh::SphereMesh(void* model, const std::string name)
        : ProceduralMesh(model, name)
    {
        setupMesh();

        bindBufferAndAttribute();
    }
    void SphereMesh::setupMesh()
    {
        //sphere's vertices are useless, so no need to push these in the m_Vertices and m_Indices
        vector<Vertex>* vertices = new vector<Vertex>;
        vector<unsigned int>* indices = new vector<unsigned int>;

        float radius = ((SphereModel*)m_OfModel)->getSphere().radius;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0 * std::_Pi) * std::sin(ySegment * std::_Pi);
                float yPos = std::cos(ySegment * std::_Pi);
                float zPos = std::sin(xSegment * 2.0 * std::_Pi) * std::sin(ySegment * std::_Pi);

                Vertex vertex;
                vertex.Position = glm::vec3(xPos, yPos, zPos) * radius;
                vertex.Normal = glm::vec3(xPos, yPos, zPos);
                vertex.TexCoords = glm::vec2(xSegment, ySegment) * radius;
                vertices->emplace_back(vertex);
            }
        }
        
        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices->emplace_back(y * (X_SEGMENTS + 1) + x);
                    indices->emplace_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices->emplace_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices->emplace_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        
        m_Vertices = vertices;
        m_Indices = indices;
    }

    void SphereMesh::drawAfterBindTextures() const
    {
        // draw mesh
        glBindVertexArray(m_VAO);
        //HZ_CORE_INFO("glBindVertexArray(m_VAO);");

        glDrawElements(GL_TRIANGLE_STRIP, m_Indices->size(), GL_UNSIGNED_INT, 0);


        glBindVertexArray(0);
    }

    CubeMesh::CubeMesh(void* model, const std::string name)
        : ProceduralMesh(model, name)
    {
        setupMesh();

        bindBufferAndAttribute();
    }
    void CubeMesh::setupMesh()
    {
        float length = ((CubeModel*)m_OfModel)->getCube().length;

        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        m_Vertices = new vector<Vertex>;
        for (int i = 0; i < 6 * 6; i++)
        {
            Vertex vertex;
            vertex.Position.x = vertices[i * 8 + 0] * length;
            vertex.Position.y = vertices[i * 8 + 1] * length;
            vertex.Position.z = vertices[i * 8 + 2] * length;
            vertex.Normal.x = vertices[i * 8 + 3];
            vertex.Normal.y = vertices[i * 8 + 4];
            vertex.Normal.z = vertices[i * 8 + 5];
            vertex.TexCoords.x = vertices[i * 8 + 6];
            vertex.TexCoords.y = vertices[i * 8 + 7];
            m_Vertices->emplace_back(vertex);
        }
    }
    void CubeMesh::drawAfterBindTextures() const
    {
        glBindVertexArray(m_VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
    }
    
    

    QuadMesh::QuadMesh(void* model, const std::string name)
        : ProceduralMesh(model, name)
    {
        setupMesh();

        bindBufferAndAttribute();
    }
    void QuadMesh::setupMesh()
    {
        float length = ((CubeModel*)m_OfModel)->getCube().length;

        float vertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        m_Vertices = new vector<Vertex>;
        for (int i = 0; i < 4; i++)
        {
            Vertex vertex;
            vertex.Position.x = vertices[i * 5 + 0] * length;
            vertex.Position.y = vertices[i * 5 + 1] * length;
            vertex.Position.z = vertices[i * 5 + 2] * length;
            vertex.TexCoords.x = vertices[i * 5 + 3];
            vertex.TexCoords.y = vertices[i * 5 + 4];
            m_Vertices->emplace_back(vertex);
        }
    }
    void QuadMesh::drawAfterBindTextures() const
    {
        glBindVertexArray(m_VAO);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(0);
    }

}