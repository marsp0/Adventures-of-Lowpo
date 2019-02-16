
#include <iostream>

#include "Octree.hpp"

Octree::Octree(int maxLevel, glm::vec3 center, float halfWidth)
{
    this->node = this->BuildOctree(maxLevel, center, halfWidth);
    std::cout << "built the octree" << std::endl;
}

std::shared_ptr<OctreeNode> Octree::BuildOctree(int maxLevel, glm::vec3 center, float halfWidth)
{
    if (maxLevel < 0)
    {
        return nullptr;
    }
    else
    {
        std::shared_ptr<OctreeNode> node = std::make_shared<OctreeNode>(center, halfWidth);
        glm::vec3 offset;
        float step = halfWidth * 0.5;
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x + step,center.y + step, center.z + step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x - step,center.y + step, center.z + step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x - step,center.y - step, center.z + step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x + step,center.y - step, center.z + step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x + step,center.y + step, center.z - step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x - step,center.y + step, center.z - step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x - step,center.y - step, center.z - step),step));
        node->children.push_back(this->BuildOctree(maxLevel-1,glm::vec3(center.x + step,center.y - step, center.z - step),step));
        return node;
    }
}

bool Octree::Insert(glm::vec3 object)
{
    return this->node->Insert(object);
}

void Octree::CheckCollisions(std::shared_ptr<OctreeNode> node, std::vector<std::shared_ptr<OctreeNode>>& ancesstors, int currentDepth)
{
    ancesstors.push_back(node);
    currentDepth++;

    for (int i = 0; i < currentDepth; i++)
    {
        glm::vec3 first;
        glm::vec3 second;
        std::cout << ancesstors[i]->objects.size() << std::endl;
        for (int j = 0; j < ancesstors[i]->objects.size() ; j++)
        {
            first = ancesstors[i]->objects[j];
            for (int k = 0;k < node->objects.size(); k++)
            {
                second = node->objects[k];
                if (first == second)
                {
                    break;
                }
                std::cout << "Checking collision between " << first.x << " and " << second.x << std::endl;
            }
        }
    }

    for (int i = 0; i < node->children.size() ; i++)
    {
        if (node->children[i])
        {
            this->CheckCollisions(node->children[i], ancesstors, currentDepth);
        }
    }
}

void Octree::Remove(glm::vec3 object)
{
    this->node->Remove(object);
}