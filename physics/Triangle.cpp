#include "Triangle.hpp"

#include <iostream>

Triangle::Triangle( glm::vec3 center, 
                    glm::vec3 normal, 
                    std::vector<glm::vec3> points, 
                    ColliderType colliderType,
                    DynamicType dynamicType) : 
            Collider(center, colliderType, dynamicType), normal(normal), points(points)
{

}

std::vector<glm::vec3> Triangle::GetPoints()
{
    return this->points;
}