#include <GL/glew.h>
#include "Mesh.hpp"

Mesh::Mesh(unsigned int vertexArray, unsigned int vertexBuffer, unsigned int vertexCount)
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