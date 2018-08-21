#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "stb_image.hpp"
#include "Terrain.hpp"

Terrain::Terrain(unsigned int vbo, unsigned int ibo, int indexCount, Transform transform) : 
    transform(transform)
{
    this->vertexBuffer = vbo;
    this->indexBuffer = ibo;
    this->indexCount = indexCount;
}

void Terrain::Render()
{
    this->Bind();
    glDrawElements(GL_TRIANGLES,this->indexCount, GL_UNSIGNED_INT, (void*)0);
    std::cout << "after render but before unbind " << glGetError()<< " " << this->indexCount << std::endl;
    this->Unbind();
}

void Terrain::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
}

void Terrain::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}