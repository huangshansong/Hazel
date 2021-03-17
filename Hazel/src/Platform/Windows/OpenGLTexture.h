#pragma once

#include "hzpch.h"

namespace Hazel {
    //write this code: stbi_set_flip_vertically_on_load(true);  before calling these functions, if needed

    unsigned int textureFromFile(const std::string& path, bool gamma = false);

    unsigned int textureFromHDRFile(const std::string& path);

    unsigned int cubemapFromFiles(const std::vector<std::string>& paths);

    unsigned int HDR2Cubemap(unsigned int HDRTexture);

    //call glViewport() after this function called
    std::vector<unsigned int> Cubemap2IBL(unsigned int Cubemap);
}