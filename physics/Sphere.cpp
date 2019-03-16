#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 center, float radius, ColliderType colliderType, DynamicType dynamicType) : 
        Collider(center, colliderType, dynamicType), radius(radius)
{

}

std::vector<glm::vec3> Sphere::GetPoints()
{

}