#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <math.h>
#include "stb_image.hpp"
#include "Terrain.hpp"

Terrain::Terrain(unsigned int vao, unsigned int vbo, int vertexCount, 
                Transform transform, std::shared_ptr<std::vector<std::vector<float>>> heightmap,
                float worldWidth, float worldLength,
                glm::vec3 position) : 
                transform(transform),position(position),
                cellWidth(worldWidth), cellLength(worldLength)
{
    this->vertexArray = vao;
    this->vertexBuffer = vbo;
    this->vertexCount = vertexCount;
    this->heightmap = heightmap;
    this->transform.SetPosition(position);
}

void Terrain::Render()
{
    this->Bind();
    glDrawArrays(GL_TRIANGLES,0, this->vertexCount);
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
    // std::cout << this->heightmap->size() << std::endl;
    // std::cout << (*heightmap)[0].size() << std::endl;
    // std::cout << std::endl;
    // make it in range of the gridsize
    z = -z/this->cellLength;
    x /= this->cellWidth;


    int xUp = std::ceil(x);
    int xDown = std::round(x);
    int zUp = std::ceil(z);
    int zDown = std::round(z);
    float denominator;
    float w1;
    float w2;
    float w3;
    // TODO (Martin) : optimize terrain walking and make it smoother.
    denominator = (zUp - zUp) * (xDown - xUp) + (xUp - xDown) * (zDown - zUp);
    w1 = (zUp - zUp) * (x - xUp) + (xUp - xDown) * (z - zUp);
    w2 = (zUp - zDown) * (x - xUp) + (xDown - xUp) * (z - zUp);
    w3 = 1 - w1 - w2;
    if (w3 < 0 || w2 < 0 || w1 < 0)
    {
        denominator = (zDown - zUp) * (xDown - xUp) + (xUp - xUp) * (zDown - zUp);
        w1 = (zDown - zUp) * (x - xUp) + (xUp - xUp) * (z - zUp);
        w2 = (zUp - zDown) * (x - xUp) + (xDown - xUp) * (z - zUp);
        w3 = 1 - w2 - w3;
        float result = w1 * (*heightmap)[zDown][xDown] + w2 * (*heightmap)[zDown][xUp] * w3 * (*heightmap)[zUp][xUp];
        return result;
    }
    else
    {
        float result = w1 * (*heightmap)[zDown][xDown] + w2 * (*heightmap)[zUp][xDown] * w3 * (*heightmap)[zUp][xUp];
        return result;
    }
}

glm::vec3 Terrain::GetPosition()
{
    return this->position;
}