#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

struct Vertex
{
    public:
        Vertex(const glm::vec3& position, const glm::vec2& texCoord, const glm::vec3& normal);
        glm::vec3* GetPosition();
        glm::vec2* GetTexCoord();
        glm::vec3* GetNormal();
    private:
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
};


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