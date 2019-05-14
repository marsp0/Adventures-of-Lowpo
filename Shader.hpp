#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader
{
    public:
        Shader(std::string vertexPath, std::string fragmentPath);
        // Copy Constructor
        Shader(const Shader& other);
        void Use();
        unsigned int Load(std::string vertexPath, std::string fragmnetPath);

        void SetFloat(const GLchar* name, GLfloat value);
        void SetInt(const GLchar* name, GLint value);
        
        void SetVector2f(const GLchar* name, GLfloat x, GLfloat y);
        void SetVector2f(const GLchar* name, const glm::vec2& value);

        void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
        void SetVector3f(const GLchar* name, const glm::vec3& value);

        void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void SetVector4f(const GLchar* name, const glm::vec4& value);

        void SetMat4(const GLchar* name, const glm::mat4& value);
        
        GLuint ID;
};