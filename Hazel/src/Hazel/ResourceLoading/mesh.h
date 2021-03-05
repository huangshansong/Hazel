#pragma once

#include "hzpch.h"

#include <glm.hpp>

#include "Material.h"
#include "shader.h"

using namespace std;

namespace Hazel {

    struct Vertex {
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

    class Mesh {
    public:
        
        // constructor
        Mesh(vector<Vertex>* vertices, vector<unsigned int>* indices, vector<Texture>* textures);

        // render the mesh
        void draw();

        Shader* getShader() { return m_Shader; }

    public:
        class ShaderSetter
        {
            friend class Level;
        private:
            inline static void setShader(Mesh* mesh, Shader* shader) { mesh->m_Shader = shader; }
        };

    private:
        
        // render data 
        unsigned int VAO;
        unsigned int VBO, EBO;
        Shader* m_Shader;
        // mesh data
        vector<Vertex>* m_Vertices;
        vector<unsigned int>* m_Indices;
        vector<Texture>* m_Textures;

        // initializes all the buffer objects/arrays
        void setupMesh();
    };

}

