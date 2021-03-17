#pragma once 
#include "hzpch.h"

#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Hazel/HObject.h"

#include "mesh.h"

namespace Hazel 
{

    class HAZEL_API Model : public HObject
    {
    public:
        Model(void* actor, glm::vec3 scale = glm::vec3(1.0f));

        virtual ~Model() = default;

        void setName(std::string& name) { m_Name = name; }

        void setActor(void* actor) { m_OfActor = actor; }

        void setModelUniversalMaterial(std::shared_ptr<Material>& material);

        void setModelUniversalShader(std::shared_ptr<Shader>& shader);
        
        void setScale(glm::vec3& scale) { m_Scale = scale; }

        const void* getOfActor() const { return m_OfActor; }

        const glm::mat4& getModelTransformMatrix() const { return m_ModelTransformMatrix; }

        const glm::vec3& getScale() const { return m_Scale; }


        void onUpdate();

        // draws the model, and thus all its meshes
        void draw() const;

        uint32_t selectLODbyDistance(float) const;

        std::vector<std::vector<std::shared_ptr<Mesh>>> m_LODs;

        std::string m_Name = "Unnamed";

        glm::mat4 m_ModelTransformMatrix;

        glm::vec3 m_Scale;

    protected: 
        
        // model data 
        void* m_OfActor = nullptr;
        
    };

}