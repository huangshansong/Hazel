#pragma once
#include "hzpch.h"

#include <Glad/glad.h>
#include <glm.hpp>

namespace Hazel {

    class Shader
    {
    public:
        class Interface
        {
            friend class Mesh;
        private:
            static Shader* create
            (const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) 
            { 
                return new Shader(vertexPath, fragmentPath, geometryPath); 
            }
            static void destroy(Shader* shader) { shader->~Shader(); }

            static unsigned int getID(Shader* shader) { return shader->m_ID; }
            static std::string& getVertexShaderPath(Shader* shader) { return shader->m_VertexPath; }

            // activate the shader
            // ------------------------------------------------------------------------
            static void use(Shader* shader) { glUseProgram(shader->m_ID); }
            // utility uniform functions
            // ------------------------------------------------------------------------
            static void setBool(Shader* shader, const std::string& name, bool value)
            {
                glUniform1i(glGetUniformLocation(shader->m_ID, name.c_str()), (int)value);
            }
            // ------------------------------------------------------------------------
            static void setInt(Shader* shader, const std::string& name, int value)
            {
                glUniform1i(glGetUniformLocation(shader->m_ID, name.c_str()), value);
            }
            // ------------------------------------------------------------------------
            static void setFloat(Shader* shader, const std::string& name, float value) 
            {
                glUniform1f(glGetUniformLocation(shader->m_ID, name.c_str()), value);
            }
            // ------------------------------------------------------------------------
            static void setVec2(Shader* shader, const std::string& name, const glm::vec2& value) 
            {
                glUniform2fv(glGetUniformLocation(shader->m_ID, name.c_str()), 1, &value[0]);
            }
            static void setVec2(Shader* shader, const std::string& name, float x, float y) 
            {
                glUniform2f(glGetUniformLocation(shader->m_ID, name.c_str()), x, y);
            }
            // ------------------------------------------------------------------------
            static void setVec3(Shader* shader, const std::string& name, const glm::vec3& value) 
            {
                glUniform3fv(glGetUniformLocation(shader->m_ID, name.c_str()), 1, &value[0]);
            }
            static void setVec3(Shader* shader, const std::string& name, float x, float y, float z)
            {
                glUniform3f(glGetUniformLocation(shader->m_ID, name.c_str()), x, y, z);
            }
            // ------------------------------------------------------------------------
            static void setVec4(Shader* shader, const std::string& name, const glm::vec4& value) 
            {
                glUniform4fv(glGetUniformLocation(shader->m_ID, name.c_str()), 1, &value[0]);
            }
            static void setVec4(Shader* shader, const std::string& name, float x, float y, float z, float w)
            {
                glUniform4f(glGetUniformLocation(shader->m_ID, name.c_str()), x, y, z, w);
            }
            // ------------------------------------------------------------------------
            static void setMat2(Shader* shader, const std::string& name, const glm::mat2& mat) 
            {
                glUniformMatrix2fv(glGetUniformLocation(shader->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
            }
            // ------------------------------------------------------------------------
            static void setMat3(Shader* shader, const std::string& name, const glm::mat3& mat) 
            {
                glUniformMatrix3fv(glGetUniformLocation(shader->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
            }
            // ------------------------------------------------------------------------
            static void setMat4(Shader* shader, const std::string& name, const glm::mat4& mat) 
            {
                glUniformMatrix4fv(glGetUniformLocation(shader->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
            }
        };
    private:
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        ~Shader() = default;

        unsigned int m_ID;
        std::string m_VertexPath;
        std::string m_FragmentPath;
        std::string m_GeometryPath;

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(GLuint shader, std::string type);
    };
}
