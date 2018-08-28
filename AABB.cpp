#include "AABB.hpp"
#include <iostream>

AABB::AABB(glm::vec3 min, glm::vec3 max)
{
    this->min = min;
    this->max = max;
}

bool AABB::HandleCollision(const AABB& other)
{
    if (this->max.x < other.min.x || this->min.x > other.max.x)
        return false;
    if (this->max.y < other.min.y || this->min.y > other.max.y)
        return false;
    if (this->max.z < other.min.z || this->min.z > other.max.z)
        return false;
    return true;
}