#pragma once

#include "Collider.hpp"
#include "AABB.hpp"
#include "Triangle.hpp"

class Sphere : public Collider
{
    public:
        Sphere(glm::vec3 center, float radius, bool dynamic, ColliderType colliderType);
        ~Sphere();

        bool CheckCollision(std::shared_ptr<Collider> collider);
        bool CheckCollision(std::shared_ptr<Triangle> triangle);
        bool CheckCollision(std::shared_ptr<Sphere> sphere);
        bool CheckCollision(std::shared_ptr<AABB> box);
    private:
        unsigned int radius;
        
};