#include "hzpch.h"

#include <Glad/glad.h> // holds all OpenGL type declarations
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Hazel/Actors/camera.h"
#include "Hazel/Application.h"
#include "Hazel/Log.h"
#include "Hazel/Core.h"

#include "model.h"
#include "QuixelModel.h"
#include "Material.h"
#include "shader.h"
#include "mesh.h"

using namespace std;
namespace Hazel
{

    Mesh::Mesh(void* model, const std::string name)
        : m_OfModel(model), m_Name(name)
    {
    }

    Mesh::~Mesh()
    {
        delete m_Vertices;
        delete m_Indices;
    }

    // render the mesh
    void Mesh::draw() const
    {
        bindTexture();

        setShaderUniforms();

        drawAfterBindTextures();
    }

    void Mesh::bindTexture() const
    {
        m_Shader->use();
        
        const unsigned int TYPES = sizeof(textureTypes) / sizeof(char*);
        unsigned int TypeCount[TYPES];
        for (unsigned int i = 0; i < TYPES; i++) TypeCount[i] = 0;
        
        GLint totalCount = 0;
        const vector<Texture*>& textures = m_Material->getTextures();
        for (const Texture * texture : textures)
        {
            glActiveTexture(GL_TEXTURE0 + totalCount);
            glBindTexture(GL_TEXTURE_2D, texture->id);
            string textureName = textureTypes[texture->type];
            textureName += '[' + to_string(TypeCount[texture->type]) + ']';
            //HZ_CORE_INFO(textureName);
            m_Shader->setInt(textureName, totalCount);
            TypeCount[texture->type] ++;
            totalCount++;
        }

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::setShaderUniforms() const
    {
        m_Shader->use();
        m_Shader->setMat4("model", ((Model*)m_OfModel)->getModelTransformMatrix());
        const Camera* camera = ((Level*)((Actor*)((Model*)m_OfModel)->getOfActor())->getOfLevel())->getCamera();
        m_Shader->setMat4("view", camera->getCameraViewMatrix());
        m_Shader->setMat4("projection", camera->getCameraProjectionMatrix());

        m_Shader->setVec3("camPos", camera->getTransform());

        glm::vec3 lightDirection = glm::vec3(1.0f, 1.0f, 1.0f);
        m_Shader->setVec3("lightDirection", lightDirection);
        glm::vec3 lightColor = glm::vec3(100.0f, 100.0f, 100.0f);
        m_Shader->setVec3("lightColor", lightColor);
        float heightScale = 1.0f;
        m_Shader->setFloat("heightScale", heightScale);


    }

    // initializes all the buffer objects/arrays
    void Mesh::bindBufferAndAttribute()
    {
        if (!m_Vertices->empty())
        {
            // create buffers/arrays
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);

            glBindVertexArray(m_VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, m_Vertices->size() * sizeof(Vertex), m_Vertices->data(), GL_STATIC_DRAW);

            if (!m_Indices->empty())
            {
                glGenBuffers(1, &m_EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices->size() * sizeof(unsigned int), m_Indices->data(), GL_STATIC_DRAW);
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
}