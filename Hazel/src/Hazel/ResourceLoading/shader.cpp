#include "hzpch.h"

#include <Glad/glad.h>

#include "Hazel/Core.h"
#include "Hazel/Log.h"

#include "FileSystem.h"
#include "shader.h"

using namespace std;
namespace Hazel
{
    // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
    Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
    {
        m_VertexPath = vertexPath;
        m_FragmentPath = fragmentPath;
        if (geometryPath != nullptr)
        {
            m_GeometryPath = geometryPath;
        }
        else 
        {
            m_GeometryPath = "";
        }
        init();  
    }

    Shader::Shader(const char * directory)
    {
        string path = directory;
        path = path + "Universal";
        m_VertexPath = path + ".vs";
        m_FragmentPath = path + ".fs";
        m_GeometryPath = "";

        init();
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_ID);
    }

    void Shader::init()
    {
        m_ID = 0;
        // 1. retrieve the m_VertexShader/m_FragmentShader source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(FileSystem::getPath(m_VertexPath));
            fShaderFile.open(FileSystem::getPath(m_FragmentPath));
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if m_GeometryShader shader path is present, also load a m_GeometryShader shader
            if (m_GeometryPath != "")
            {
                gShaderFile.open(FileSystem::getPath(m_GeometryPath));
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            HZ_CORE_INFO("shader file not successfully read!");
            return;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int m_VertexShader, m_FragmentShader;
        // m_VertexShader shader
        m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_VertexShader, 1, &vShaderCode, NULL);
        glCompileShader(m_VertexShader);
        checkCompileErrors(m_VertexShader, "VERTEX");
        // m_FragmentShader Shader
        m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_FragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(m_FragmentShader);
        checkCompileErrors(m_FragmentShader, "FRAGMENT");
        // if m_GeometryShader shader is given, compile m_GeometryShader shader
        unsigned int m_GeometryShader;
        if (m_GeometryPath != "")
        {
            const char* gShaderCode = geometryCode.c_str();
            m_GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(m_GeometryShader, 1, &gShaderCode, NULL);
            glCompileShader(m_GeometryShader);
            checkCompileErrors(m_GeometryShader, "GEOMETRY");
        }
        // shader Program
        m_ID = glCreateProgram();
        glAttachShader(m_ID, m_VertexShader);
        glAttachShader(m_ID, m_FragmentShader);
        if (m_GeometryPath != "")
            glAttachShader(m_ID, m_GeometryShader);
        glLinkProgram(m_ID);
        checkCompileErrors(m_ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(m_VertexShader);
        glDeleteShader(m_FragmentShader);
        if (m_GeometryPath != "")
            glDeleteShader(m_GeometryShader);

    }

    // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
    void Shader::checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}