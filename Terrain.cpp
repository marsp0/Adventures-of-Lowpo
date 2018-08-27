#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <math.h>
#include "stb_image.hpp"
#include "Terrain.hpp"

Terrain::Terrain(unsigned int vao, unsigned int vbo, 
                unsigned int ibo, int indexCount, 
                Transform transform, std::shared_ptr<std::vector<std::vector<float>>> heightmap,
                float worldWidth, float worldHeight, float worldLength,
                glm::vec3 position) : 
                worldWidth(worldWidth), worldHeight(worldHeight), 
                worldLength(worldLength), transform(transform),
                position(position)
{
    this->vertexArray = vao;
    this->vertexBuffer = vbo;
    this->indexBuffer = ibo;
    this->indexCount = indexCount;
    this->heightmap = heightmap;
    this->transform.SetPosition(position);
}

void Terrain::Render()
{
    this->Bind();
    glDrawElements(GL_TRIANGLES,this->indexCount, GL_UNSIGNED_INT, (void*)0);
    this->Unbind();
}

void Terrain::Bind()
{
    glBindVertexArray(this->vertexArray);
}

void Terrain::Unbind()
{
    glBindVertexArray(0);
}

float Terrain::GetHeight(float x, float z)
{
    x += this->worldWidth/2;
    z += this->worldLength/2;

    x /= this->worldWidth;
    z /= this->worldLength;

    x *= (*(this->heightmap))[0].size();
    z *= this->heightmap->size();

    // std::cout << "x and z are "<< std::endl;
    // std::cout << x << std::endl;
    // std::cout << z << std::endl;

    float xUp = std::ceil(x);
    float xDown = std::round(x);

    float zUp = std::ceil(z);
    float zDown = std::round(z);

    int resultX, resultZ;

    if ((x - xDown) < (xUp - x))
        resultX = xDown;
    else
        resultX = xUp;

    if ((z - zDown) < (zUp - z))
        resultZ = zDown;
    else
        resultZ = zUp;
    return (*(this->heightmap))[resultZ][resultX];
}

glm::vec3 Terrain::GetPosition()
{
    return this->position;
}