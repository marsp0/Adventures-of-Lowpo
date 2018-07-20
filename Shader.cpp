#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // Load a shader from file to a c string;
    const GLchar* vertexSource;
    std::ifstream vertexFile(vertexPath);
    std::stringstream vertexStringStream;
    std::string vertexStringSource;
    vertexStringStream << vertexFile.rdbuf();
    vertexStringSource = vertexStringStream.str();
    vertexSource = vertexStringSource.c_str();
    
    const GLchar* fragmentSource;
    std::ifstream framgentFile(fragmentPath);
    std::stringstream fragmentStringStream;
    std::string fragmentStringSource;
    fragmentStringStream << framgentFile.rdbuf();
    fragmentStringSource = fragmentStringStream.str();
    fragmentSource = fragmentStringSource.c_str();

    GLint success;
    GLchar infoLog[512];

    // compile vertex shader
    unsigned int vertexID;
    vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID,1,&vertexSource,NULL);
    glCompileShader(vertexID);
    glGetShaderiv(vertexID,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(vertexID,512,NULL,infoLog);
        std::cout << "VERTEX SHADER :: COMPILE ERROR :: " << infoLog << std::endl;
    }

    unsigned int fragmentID;
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID,1,&fragmentSource,NULL);
    glCompileShader(fragmentID);
    glGetShaderiv(fragmentID,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentID,512,NULL, infoLog);
        std::cout << "FRAGMENT SHADER :: COMPILE ERROR :: " << infoLog << std::endl;
    }

    // program
    this->ID = glCreateProgram();
    glUseProgram(this->ID);
    glAttachShader(this->ID,vertexID);
    glAttachShader(this->ID,fragmentID);
    glLinkProgram(this->ID);
    glGetProgramiv(this->ID,GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->ID, 512,NULL, infoLog);
        std::cout << "PROGRAM SHADER :: LINK ERROR :: " << infoLog << std::endl;
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

void Shader::Use()
{
    glUseProgram(this->ID);
}

void Shader::SetFloat(const GLchar* name, GLfloat value)
{
    glUniform1f(glGetUniformLocation(this->ID, name),value);
}

void Shader::SetInt(const GLchar* name, GLint value)
{
    glUniform1f(glGetUniformLocation(this->ID, name),value);
}

void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y)
{
    glUniform2f(glGetUniformLocation(this->ID, name),x,y);
}

void Shader::SetVector2f(const GLchar* name, const glm::vec2& value)
{
    glUniform2f(glGetUniformLocation(this->ID, name),value.x, value.y);
}

void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
    glUniform3f(glGetUniformLocation(this->ID,name),x,y,z);
}

void Shader::SetVector3f(const GLchar* name, const glm::vec3& value)
{
    glUniform3f(glGetUniformLocation(this->ID,name),value.x,value.y,value.z);
}

void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glUniform4f(glGetUniformLocation(this->ID,name),x,y,z,w);
}

void Shader::SetVector4f(const GLchar* name, const glm::vec4& value)
{
    glUniform4f(glGetUniformLocation(this->ID,name),value.x,value.y,value.z,value.w);
}

void Shader::SetMat4(const GLchar* name, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID,name),1,GL_FALSE, glm::value_ptr(value));
}
