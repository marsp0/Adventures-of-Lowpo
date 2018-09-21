#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "ObjectTransform.hpp"

class Terrain
{
    public:
        Terrain(unsigned int vao, unsigned int vbo, 
                int vertexCount, Transform transform, 
                std::shared_ptr<std::vector<std::vector<float>>> heightmap,
                float cellWidth, float cellLength,
                glm::vec3 position);
        
        void        Render();
        void        Bind();
        void        Unbind();
        float       GetHeight(float x, float z);
        glm::vec3   GetPosition();


        Transform       transform;

    private:
        int             vertexCount;
        unsigned int    vertexBuffer;
        unsigned int    vertexArray;
        float           cellWidth;
        float           cellLength;
        glm::vec3       position;
        std::shared_ptr<std::vector<std::vector<float>>> heightmap;
};