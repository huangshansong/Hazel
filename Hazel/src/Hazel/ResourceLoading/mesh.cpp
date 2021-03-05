#include "hzpch.h"

#include <Glad/glad.h> // holds all OpenGL type declarations
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Hazel/Log.h"
#include "Hazel/Core.h"

#include "Material.h"
#include "shader.h"
#include "mesh.h"

using namespace std;
namespace Hazel
{
    Mesh::Mesh(vector<Vertex>* vertices, vector<unsigned int>* indices, vector<Texture>* textures)
    {
        this->m_Vertices = vertices;
        this->m_Indices = indices;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();

        this->m_Textures = textures;

    }

    // render the mesh
    void Mesh::draw()
    {
        m_Shader->use();
        //HZ_CORE_INFO("shader->use();");

        if (m_Shader->m_ShaderType == ShaderType::PBR)
        {
            // bind appropriate textures
            GLint albedoIndices[32];
            GLint albedoNr = 0;
            GLint normalIndices[32];
            GLint normalNr = 0;
            GLint emmisionIndices[32];
            GLint emmisionNr = 0;
            GLint metallicIndices[32];
            GLint metallicNr = 0;
            GLint roughnessIndices[32];
            GLint roughnessNr = 0;
            GLint aoIndices[32];
            GLint aoNr = 0;
            GLint displacementIndices[32];
            GLint displacementNr = 0;
            for (unsigned int i = 0; i < (*m_Textures).size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, (*m_Textures)[i].id);

                string name = (*m_Textures)[i].type;
                if (name == "texture_albedo")
                    albedoIndices[albedoNr++] = i;
                else if (name == "texture_normal")
                    normalIndices[normalNr++] = i;
                else if (name == "texture_emmision")
                    emmisionIndices[emmisionNr++] = i;
                else if (name == "texture_metallic")
                    metallicIndices[metallicNr++] = i;
                else if (name == "texture_roughness")
                    roughnessIndices[roughnessNr++] = i;
                else if (name == "texture_ao")
                    aoIndices[aoNr++] = i;
                else if (name == "texture_displacement")
                    displacementIndices[displacementNr++] = i;
            }
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_albedo"), albedoNr, albedoIndices);
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_normal"), normalNr, normalIndices);
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_emmision"), emmisionNr, emmisionIndices);
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_metallic"), metallicNr, metallicIndices);
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_roughness"), roughnessNr, roughnessIndices);
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_ao"), aoNr, aoIndices);
            glUniform1iv(glGetUniformLocation(m_Shader->m_ID, "texture_displacement"), displacementNr, displacementIndices);
            //HZ_CORE_INFO("glUniform1iv();");

        }
        else if (m_Shader->m_ShaderType == ShaderType::Blinn_Phong)
        {
            // bind appropriate textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int ambientLightNr = 1;
            for (unsigned int i = 0; i < (*m_Textures).size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                glBindTexture(GL_TEXTURE_2D, (*m_Textures)[i].id); // and bind the texture
                // retrieve texture number (the N in diffuse_textureN)
                string number;
                string name = (*m_Textures)[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to stream
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to stream
                else if (name == "texture_ambientLight")
                    number = std::to_string(ambientLightNr++); // transfer unsigned int to stream

                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(m_Shader->m_ID, (name + number).c_str()), i);
                //HZ_CORE_INFO("glUniform1i();"); 
                
            }
        }

        // draw mesh
        glBindVertexArray(VAO);
        //HZ_CORE_INFO("glBindVertexArray(VAO);");
        if (!(*m_Indices).empty())
        {
            glDrawElements(GL_TRIANGLES, (*m_Indices).size(), GL_UNSIGNED_INT, 0);
            //HZ_CORE_INFO("glDrawElements(GL_TRIANGLES, (*m_Indices).size(), GL_UNSIGNED_INT, 0);");
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, (*m_Vertices).size());
            //HZ_CORE_INFO("glDrawArrays(GL_TRIANGLES, 0, (*m_Vertices).size());");
        }
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    // initializes all the buffer objects/arrays
    void Mesh::setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, (*m_Vertices).size() * sizeof(Vertex), &((*m_Vertices)[0]), GL_STATIC_DRAW);

        if (!(*m_Indices).empty())
        {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*m_Indices).size() * sizeof(unsigned int), &((*m_Indices)[0]), GL_STATIC_DRAW);
        }

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        /*
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        */
        glBindVertexArray(0);
    }
}