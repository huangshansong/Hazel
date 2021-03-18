#include "hzpch.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "Hazel/Actors/Actor.h"
#include "Hazel/Levels/Level.h"

#include "Material.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"

using namespace std;
namespace Hazel
{
    Model::Model(void* actor, glm::vec3 scale)
        : m_OfActor(actor), m_Scale(scale)
    {
        if (actor != nullptr)
        {
            ((Actor*)actor)->m_Model = shared_ptr<Model>(this);
        }
    }

    uint32_t Model::selectLODbyDistance(float distance) const
    {
        uint32_t LODNumber = (distance + 1E-5F) / 10.0f + 1E-5F;

        return LODNumber <= m_LODs.size() - 1 ? LODNumber : m_LODs.size() - 1;
    }

    void Model::onUpdate()
    {
        m_ModelMatrix = glm::translate(glm::mat4(((Actor*)m_OfActor)->getRotation()), ((Actor*)m_OfActor)->getTransform());
    }

    void Model::setModelUniversalMaterial(shared_ptr<Material>& material)
    {
        for (unsigned int i = 0; i < m_LODs.size(); i++)
        {
            for (shared_ptr<Mesh>& mesh : m_LODs[i])
            {
                mesh->setMaterial(material);
            }
        }
    }

    void Model::setModelUniversalShader(shared_ptr<Shader>& shader)
    {
        for (unsigned int i = 0; i < m_LODs.size(); i++)
        {
            for (shared_ptr<Mesh>& mesh : m_LODs[i])
            {
                mesh->setShader(shader);
            }
        }
    }

    // draws the model, and thus all its meshes
    void Model::draw() const
    {
        //draw LOD according to distance between actor and camera

        float distance = glm::distance(((Actor*)m_OfActor)->getTransform(), ((Actor*)((Level*)((Actor*)m_OfActor)->getOfLevel())->getCamera())->getTransform());
        for (shared_ptr<Mesh> mesh : m_LODs[selectLODbyDistance(distance)])
        {
            mesh->draw();
        }
        //HZ_CORE_INFO(m_Meshes.size());
    }
}