#pragma once
#include "hzpch.h"

#include <glm.hpp>

#include "Hazel/HObject.h"

#include "Material.h"
#include "shader.h"

namespace Hazel 
{

    struct Vertex
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };

    class HAZEL_API Mesh : public HObject
    {
    public:
        Mesh(void* model, std::vector<Vertex>* vertices = nullptr, std::vector<unsigned int>* indices = nullptr, const std::string name = "Unnamed");

        virtual ~Mesh();

        void setName(std::string& name) { m_Name = name; }

        void setMaterial(std::shared_ptr<Material>& material) { m_Material = material; }

        void setShader(std::shared_ptr<Shader>& shader) { m_Shader = shader; }

        const void* getOfModel() const { return m_OfModel; }
        
        // render the mesh
        void draw() const;

        std::string m_Name;

        std::shared_ptr<Material> m_Material;

        std::shared_ptr<Shader> m_Shader;

    protected:
        virtual void setupMesh() {};

        //override this if the mesh is without indices!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        virtual void drawAfterBindTextures() const;

        // initializes all the buffer objects/arrays, textures, and shader
        void bindBufferAndAttribute();

        void bindTexture() const;

        void setShaderUniforms() const;


        void* m_OfModel = nullptr;
        // render data 
        unsigned int m_VAO;
        unsigned int m_VBO, m_EBO;
        // mesh data
        std::vector<Vertex>* m_Vertices;
        std::vector<unsigned int>* m_Indices;

    };

}

