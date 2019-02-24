
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
    for (int i = 0; i < ancesstors.size() ; i++)
    {
        glm::vec3 first;
        glm::vec3 second;
        for (int j = 0; j < ancesstors[i]->objects.size(); j++)
        {
            first = ancesstors[i]->objects[j];
            for (int k = 0; k < node->objects.size(); k++)
            {
                second = node->objects[k];
                if (first == second)
                {
                    continue;
                }
                std::cout << "Checking " << first.x << " " << second.x << std::endl;
                std::cout << "Checking " << first.y << " " << second.y << std::endl;
                std::cout << "Checking " << first.z << " " << second.z << std::endl;
                std::cout << std::endl;
            }
        }
    }
    for (int i = 0; i < node->children.size(); i++)
    {
        if (node->children[i])
        {
            this->CheckCollisions(node->children[i],ancesstors,currentDepth);
        }
    }
    ancesstors.pop_back();
}

void Octree::Remove(glm::vec3 object)
{
    this->node->Remove(object);
}