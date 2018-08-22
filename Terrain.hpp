#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "ObjectTransform.hpp"

class Terrain
{
    public:
        Terrain(unsigned int vao, unsigned int vbo, unsigned int ibo, int indexCount, 
                Transform transform, 
                std::shared_ptr<std::vector<std::vector<float>>> heightmap,
                float worldWidth, float worldHeight, float worldLength);
        
        void    Render();
        void    Bind();
        void    Unbind();
        float   GetHeight(float x, float z);

        Transform       transform;
    private:
        int             indexCount;
        unsigned int    vertexBuffer;
        unsigned int    vertexArray;
        unsigned int    indexBuffer;
        float           worldWidth;
        float           worldHeight;
        float           worldLength;

        std::shared_ptr<std::vector<std::vector<float>>> heightmap;
};