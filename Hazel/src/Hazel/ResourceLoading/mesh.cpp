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
    void Mesh::init(vector<Vertex>* vertices, vector<unsigned int>* indices, vector<Texture*>& textures, const string& name)
    {
        m_Vertices = vertices;
        m_Indices = indices;
        m_Name = name;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        bindBufferAndAttribute(*vertices, *indices);  

        m_Material = createMaterial(textures);

        bindShader();

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
        Shader::Interface::use(m_Shader);
        
        const unsigned int TYPES = sizeof(textureTypes) / sizeof(char*);
        unsigned int TypeCount[TYPES];
        for (unsigned int i = 0; i < TYPES; i++) TypeCount[i] = 0;
        
        GLint totalCount = 0;
        const vector<Texture*>& textures = getMaterialTextures(m_Material);
        for (const Texture * texture : textures)
        {
            glActiveTexture(GL_TEXTURE0 + totalCount);
            glBindTexture(GL_TEXTURE_2D, texture->id);
            string textureName = textureTypes[texture->type];
            textureName += '[' + to_string(TypeCount[texture->type]) + ']';
            HZ_CORE_INFO(textureName);
            Shader::Interface::setInt(m_Shader, textureName, totalCount);
            TypeCount[texture->type] ++;
            totalCount++;
        }

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::bindShader()
    {
        Model* model = (Model*)m_OfModel;
        string directory = model->getDirectory();
        const vector<Shader*>& shadersLoaded = Model::InterfaceDown::getShadersLoaded(model);

        bool meshHasUniqueShader = false;
        string shaderPath;
        for (int i = 0; i <= 1; i++)
        {
            if (i == 0)
            {
                shaderPath = directory + m_Name;
            }
            else
            {
                shaderPath = directory + "Universal";
            }
            bool skip = false;
            for (Shader* shader : shadersLoaded)
            {
                if (Shader::Interface::getVertexShaderPath(shader) == shaderPath + ".vs")
                {
                    m_Shader = shader;
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                string vertexShaderPath = shaderPath + ".vs";
                string fragmentShaderPath = shaderPath + ".fs";
                string geometryShaderPath;
                Shader* shader = Shader::Interface::create(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
                if (Shader::Interface::getID(shader) != 0)
                {
                    m_Shader = shader;
                    break;//if the mesh has its unique shader, no need to load the Universal shader 
                }
            }
        } 
    }

    void Mesh::setShaderUniforms() const
    {
        Shader::Interface::use(m_Shader);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), ((Model*)m_OfModel)->getOfActorTransform());
        Shader::Interface::setMat4(m_Shader, "model", model);
        glm::mat4 view = Application::getWindow()->getViewport()->getLevel()->getCamera()->getViewMatrix();
        Shader::Interface::setMat4(m_Shader, "view", view);
        glm::mat4 projection = glm::perspective(
            glm::radians(Application::getWindow()->getViewport()->getLevel()->getCamera()->getZoom()), 
            (float)Application::getWindow()->getViewport()->getWidth() / (float)Application::getWindow()->getViewport()->getHeight(), 
            0.1f, 100.0f);
        Shader::Interface::setMat4(m_Shader, "projection", projection);
        glm::vec3 cameraTransform = Application::getWindow()->getViewport()->getLevel()->getCamera()->getTransform();
        
        Shader::Interface::setVec3(m_Shader, "camPos", cameraTransform);
        glm::vec3 lightDirection = glm::vec3(1.0f, 1.0f, 1.0f);
        Shader::Interface::setVec3(m_Shader, "lightDirection", lightDirection);
        glm::vec3 lightColor = glm::vec3(100.0f, 100.0f, 100.0f);
        Shader::Interface::setVec3(m_Shader, "lightColor", lightColor);
        float heightScale = 1.0f;
        Shader::Interface::setFloat(m_Shader, "heightScale", heightScale);


    }

    std::vector<Texture*>& Mesh::getModelTexturesLoaded()
    {
        return Model::InterfaceDown::getTexturesLoaded((Model*)m_OfModel);
    }

    std::vector<Shader*>& Mesh::getModelShadersLoaded()
    {
        return Model::InterfaceDown::getShadersLoaded((Model*)m_OfModel);
    }

    void Mesh::drawAfterBindTextures() const
    {
        // draw mesh
        glBindVertexArray(m_VAO);
        //HZ_CORE_INFO("glBindVertexArray(VAO);");

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //ONLY MESH WITH INDICES CAN BE DRAW IN CLASS:Mesh!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        glDrawElements(GL_TRIANGLES, m_Indices->size(), GL_UNSIGNED_INT, 0);
        //HZ_CORE_INFO("glDrawElements(GL_TRIANGLES, (*m_Indices).size(), GL_UNSIGNED_INT, 0);");
        //ONLY MESH WITH INDICES CAN BE DRAW IN CLASS:Mesh!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        
        glBindVertexArray(0);

    }

    // initializes all the buffer objects/arrays
    void Mesh::bindBufferAndAttribute(vector<Vertex>& vertices, vector<unsigned int>& indices)
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        if (!indices.empty())
        {
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
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