#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

#include "BoundingVolume.hpp"
#include "OctreeNode.hpp"

class Octree {

    public:

        Octree(int maxLevel, glm::vec3 center, float halfWidth);
        std::shared_ptr<OctreeNode> BuildOctree(int maxLevel, glm::vec3 center, float halfWidth);
        bool Insert(glm::vec3 object);
        void CheckCollisions(std::shared_ptr<OctreeNode> node, std::vector<std::shared_ptr<OctreeNode>>& ancesstors, int currentDepth);
        void Remove(glm::vec3 object);
        void GetCollisions();

        std::shared_ptr<OctreeNode> node;
    private:
};