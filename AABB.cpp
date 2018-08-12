#include "AABB.hpp"

AABB::AABB()
{

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