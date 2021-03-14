#include "hzpch.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Hazel/ResourceLoading/FileSystem.h"

using namespace std;
namespace Hazel {
    unsigned int textureFromFile(const string& path, bool gamma)
    {
        int width, height, nrComponents;
        unsigned char* data = stbi_load(FileSystem::getPath(path).c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            
            stbi_image_free(data);

            return textureID;
        }
        else
        {
            stbi_image_free(data);
            return 0;
        }   
    }
}
