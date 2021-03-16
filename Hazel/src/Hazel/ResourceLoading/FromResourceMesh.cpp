#include "hzpch.h"

#include "FromResourceMesh.h"

using namespace std;
namespace Hazel
{
    FromResourceMesh::FromResourceMesh(void* model, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, const std::string name)
        : Mesh(model, name)
    {
        m_Vertices = vertices;
        m_Indices = indices;

        bindBufferAndAttribute();
    }
    void FromResourceMesh::drawAfterBindTextures() const
	{
        // draw mesh
        glBindVertexArray(m_VAO);
        //HZ_CORE_INFO("glBindVertexArray(VAO);");

        glDrawElements(GL_TRIANGLES, m_Indices->size(), GL_UNSIGNED_INT, 0);
        

        glBindVertexArray(0);
	}
}


