#include "Mesh.hpp"
// #include "OBJ_Loader.hpp"
#include <GL/glew.h>

Mesh::Mesh(GLuint vertexArray, GLuint vertexBuffer, GLuint vertexCount)
{
    this->vertexArray = vertexArray;
    this->vertexBuffer = vertexBuffer;
    this->vertexCount = vertexCount;
}

void Mesh::Draw()
{
    glDrawArrays(GL_TRIANGLES,0,this->vertexCount);
}

void Mesh::Bind()
{
    glBindVertexArray(this->vertexArray);
}

void Mesh::Unbind()
{
    glBindVertexArray(0);
}