#pragma once

#include "Collider.hpp"

class AABB : public Collider
{
    public:

        AABB(glm::vec3 center, glm::vec3 axisRadii, std::vector<glm::vec3> points, ColliderType colliderType, DynamicType dynamicType);

        glm::vec3 axisRadii;

    private:
};