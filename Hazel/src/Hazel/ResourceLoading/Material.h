#pragma once
#include "hzpch.h"

namespace Hazel
{
    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Material
    {
        std::vector<Texture> textures;
    };
}