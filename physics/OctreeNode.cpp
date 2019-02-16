#include <iostream>

#include "OctreeNode.hpp"

// *****************************
//          OctreeNode
// *****************************

OctreeNode::OctreeNode(glm::vec3 center, float halfWidth) :
    center(center), halfWidth(halfWidth)
{

}

bool OctreeNode::Insert(glm::vec3 object)
{
    // check if fully contained, if not, return false
    if (this == nullptr)
    {
        return false;
    }
    for (int i = 0; i < 3 ; i++)
    {
        float deltaDistance = object[i] - this->center[i];
        if (abs(deltaDistance) > this->halfWidth)
        {
            return false;
        }
    }
    // check if can insert in children
    for (int i = 0; i < this->children.size() ; i++)
    {
        if (this->children[i]->Insert(object))
        {
            return true;
        }
    }
    this->objects.push_back(object);
    return true;
}

void OctreeNode::Remove(glm::vec3 object)
{
    for (int i=0; i < this->objects.size() ; i++)
    {
        if (this->objects[i] == object)
        {
            this->objects.erase(this->objects.begin() + i);
        }
    }
    for (int i = 0; i < this->children.size() ; i++)
    {
        if (this->children[i]->Contains(object))
        {
            this->children[i]->Remove(object);
        }
    }
}

bool OctreeNode::Contains(glm::vec3 object)
{
    // check if fully contained, if not, return false
    for (int i = 0; i < 3 ; i++)
    {
        float deltaDistance = object[i] - this->center[i];
        if (abs(deltaDistance) > this->center[i] + this->halfWidth)
        {
            return false;
        }
    }
    return true;
}