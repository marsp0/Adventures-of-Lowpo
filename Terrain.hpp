#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ObjectTransform.hpp"

class Terrain
{
    public:
        Terrain(unsigned int vbo, unsigned int ibo, int indexCount, Transform transform);
        
        void    Render();
        float   GetPosition(int x, int z);
        void    Bind();
        void    Unbind();

        Transform       transform;
    private:
        int             indexCount;
        unsigned int    vertexBuffer;
        // GLuint  vertexArray;
        unsigned int    indexBuffer;

        std::vector<std::vector<float>> map;
};