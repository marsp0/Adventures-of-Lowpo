#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Mesh
{
    public:

        Mesh(const std::string& filePath);

        void Draw();

        void Bind();
        void Unbind();

    private:
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint vertexCount;
};