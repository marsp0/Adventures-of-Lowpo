#include "AABB.hpp"
#include <iostream>

AABB::AABB(glm::vec3 center, glm::vec3 axisRadii, ColliderType colliderType, DynamicType dynamicType) : 
                        Collider(center, colliderType, dynamicType), axisRadii(axisRadii)
{

}

std::vector<glm::vec3> AABB::GetPoints()
{
    std::vector<glm::vec3> points;
    points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z - this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z - this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z + this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y - this->axisRadii.y, this->center.z + this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z - this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z - this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x + this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z + this->axisRadii.z));
    points.push_back(glm::vec3(this->center.x - this->axisRadii.x, this->center.y + this->axisRadii.y, this->center.z + this->axisRadii.z));
    return points;
}