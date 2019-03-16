#pragma once

#include "Collider.hpp"

class Sphere : public Collider
{
    public:
        Sphere(glm::vec3 center, float radius, ColliderType colliderType, DynamicType dynamicType);
        std::vector<glm::vec3> GetPoints();
    private:
        unsigned int radius;
        
};