#pragma once

#include "Collider.hpp"

class AABB : public Collider
{
    public:

        AABB(glm::vec3 center, glm::vec3 axisRadii, ColliderType colliderType, DynamicType dynamicType);

        std::vector<glm::vec3> GetPoints();

        glm::vec3 axisRadii;

    private:
};