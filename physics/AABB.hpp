#pragma once

#include "Collider.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

class AABB : public Collider
{
    public:

        AABB(glm::vec3 center, glm::vec3 axisRadii, bool dynamic);

        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(AABB box);

        glm::vec3 axisRadii;
};