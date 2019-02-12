#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class OctreeNode
{
    public:

        OctreeNode(glm::vec3 center, float halfWidth);

        bool Insert(glm::vec3 object);
        void Remove(glm::vec3 object);
        bool Contains(glm::vec3 object);

        glm::vec3 center;
        float halfWidth;
        std::vector<std::shared_ptr<OctreeNode>> children;
        std::vector<glm::vec3> objects;

};