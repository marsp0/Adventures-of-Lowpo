#include "AABB.hpp"
#include <iostream>

AABB::AABB(glm::vec3 min, glm::vec3 max, ObjectType type) :
            type(type)
{
    this->min = min;
    this->max = max;
    // std::cout << "Min of object is " << std::endl;
    // std::cout << min.x << std::endl;
    // std::cout << min.y << std::endl;
    // std::cout << min.z << std::endl;
    // std::cout << "Max of object is " << std::endl;
    // std::cout << max.x << std::endl;
    // std::cout << max.y << std::endl;
    // std::cout << max.z << std::endl;
    // std::cout << std::endl;

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

void AABB::Update(glm::vec3 position)
{
    // std::cout << "Min of " << this->type << " object is " << std::endl;
    // std::cout << min.x << std::endl;
    // std::cout << min.y << std::endl;
    // std::cout << min.z << std::endl;
    // std::cout << "Max of " << this->type << " object is " << std::endl;
    // std::cout << max.x << std::endl;
    // std::cout << max.y << std::endl;
    // std::cout << max.z << std::endl;
    // std::cout << std::endl;
    this->max += position;
    this->min += position;
}