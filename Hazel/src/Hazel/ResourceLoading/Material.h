#pragma once
#include "hzpch.h"

#include "Hazel/HObject.h"

#include "QuixelFile.h"

namespace Hazel
{
    //copied from Assimp
    enum MyTextureType {
        /** Dummy value.
         *
         *  No texture, but the value to be used as 'texture semantic'
         *  (#aiMaterialProperty::mSemantic) for all material properties
         *  *not* related to textures.
         */
        myTextureType_NONE = 0,

        /** LEGACY API MATERIALS 
         * Legacy refers to materials which 
         * Were originally implemented in the specifications around 2000.
         * These must never be removed, as most engines support them.
         */

        /** The texture is combined with the result of the diffuse
         *  lighting equation.
         */
        myTextureType_DIFFUSE = 1,

        /** The texture is combined with the result of the specular
         *  lighting equation.
         */
        myTextureType_SPECULAR = 2,

        /** The texture is combined with the result of the ambient
         *  lighting equation.
         */
        myTextureType_AMBIENT = 3,

        /** The texture is added to the result of the lighting
         *  calculation. It isn't influenced by incoming light.
         */
        myTextureType_EMISSIVE = 4,

        /** The texture is a height map.
         *
         *  By convention, higher gray-scale values stand for
         *  higher elevations from the base height.
         */
        myTextureType_HEIGHT = 5,

        /** The texture is a (tangent space) normal-map.
         *
         *  Again, there are several conventions for tangent-space
         *  normal maps. Assimp does (intentionally) not
         *  distinguish here.
         */
        myTextureType_NORMALS = 6,

        /** The texture defines the glossiness of the material.
         *
         *  The glossiness is in fact the exponent of the specular
         *  (phong) lighting equation. Usually there is a conversion
         *  function defined to map the linear color values in the
         *  texture to a suitable exponent. Have fun.
        */
        myTextureType_SHININESS = 7,

        /** The texture defines per-pixel opacity.
         *
         *  Usually 'white' means opaque and 'black' means
         *  'transparency'. Or quite the opposite. Have fun.
        */
        myTextureType_OPACITY = 8,

        /** Displacement texture
         *
         *  The exact purpose and format is application-dependent.
         *  Higher color values stand for higher vertex displacements.
        */
        myTextureType_DISPLACEMENT = 9,

        /** Lightmap texture (aka Ambient Occlusion)
         *
         *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
         *  covered by this material property. The texture contains a
         *  scaling value for the final color value of a pixel. Its
         *  intensity is not affected by incoming light.
        */
        myTextureType_LIGHTMAP = 10,

        /** Reflection texture
         *
         * Contains the color of a perfect mirror reflection.
         * Rarely used, almost never for real-time applications.
        */
        myTextureType_REFLECTION = 11,

        /** PBR Materials
         * PBR definitions from maya and other modelling packages now use this standard.
         * This was originally introduced around 2012.
         * Support for this is in game engines like Godot, Unreal or Unity3D.
         * Modelling packages which use this are very common now.
         */

        myTextureType_BASE_COLOR = 12,
        myTextureType_NORMAL_CAMERA = 13,
        myTextureType_EMISSION_COLOR = 14,
        myTextureType_METALNESS = 15,
        myTextureType_DIFFUSE_ROUGHNESS = 16,
        myTextureType_AMBIENT_OCCLUSION = 17,

        /** Unknown texture
         *
         *  A texture reference that does not match any of the definitions
         *  above is considered to be 'unknown'. It is still imported,
         *  but is excluded from any further post-processing.
        */
        myTextureType_UNKNOWN = 18
    };
    const char* const textureTypes[] =
    {
        "none",
        "diffuseMap",
        "specularMap",
        "ambientMap",
        "emmisiveMap",
        "heightMap",
        "nomalsMap",
        "shininessMap",
        "opacityMap",
        "displacementMap",
        "lightmapMap",
        "reflectionMap",
        "albedoMap",
        "normalMap",
        "emmisionMap",
        "metallicMap",
        "roughnessMap",
        "aoMap",
        "unknown"
    };

    struct HAZEL_API Texture : public HObject
    {
        unsigned int id;
        MyTextureType type;
        std::string path;
    };
    enum class Resolution
    {
        _2K = 0,
        _4K,
        _8K
    };
    const char* const ResolutionTypes[] =
    {
        "2K",
        "4K",
        "8K"
    };

    class HAZEL_API Material : public HObject
    {
    public:
        Material() {}

        virtual ~Material();

        const std::vector<Texture*>& getTextures() const { return m_Textures; }

        std::string m_Name;

    protected:
        virtual bool loadTexture(const std::string&, std::vector<Texture*>&, MyTextureType) = 0;

        virtual void loadTexturesFromFolder(const std::string&, std::vector<Texture*>&, QuixelObjectType, Resolution, unsigned int) = 0;
        
        std::vector<Texture*> m_Textures;
        
    };

    class HAZEL_API QuixelMaterial : public Material
    {
    public:
        QuixelMaterial(const std::vector<std::string>&, QuixelObjectType, Resolution, unsigned int = 0);
        QuixelMaterial(const std::string&, QuixelObjectType, Resolution, unsigned int = 0);
        
    protected:
        virtual bool loadTexture(const std::string&, std::vector<Texture*>&, MyTextureType) override;

        virtual void loadTexturesFromFolder(const std::string&, std::vector<Texture*>&, QuixelObjectType, Resolution, unsigned int) override;

    };

    class HAZEL_API OrdinaryMaterial : public Material
    {
    public:
        OrdinaryMaterial(std::vector<Texture*>& textures) { m_Textures = textures; }

    protected:
        virtual bool loadTexture(const std::string&, std::vector<Texture*>&, MyTextureType) override { return true; }

        virtual void loadTexturesFromFolder(const std::string&, std::vector<Texture*>&, QuixelObjectType, Resolution, unsigned int) override {}

    };
    
}