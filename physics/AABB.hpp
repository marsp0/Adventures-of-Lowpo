#pragma once

#include "Collider.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

class AABB : public Collider
{
    public:

        AABB(glm::vec3 center, glm::vec3 axisRadii, bool dynamic, ColliderType colliderType);

        bool CheckCollision(std::shared_ptr<Collider> collider);
        bool CheckCollision(std::shared_ptr<Triangle> triangle);
        bool CheckCollision(std::shared_ptr<Sphere> sphere);
        bool CheckCollision(std::shared_ptr<AABB> box);

        glm::vec3 axisRadii;
};