#pragma once

#include "Collider.hpp"
#include "AABB.hpp"
#include "Triangle.hpp"

class Sphere : public Collider
{
    public:
        Sphere(glm::vec3 center, float radius, bool dynamic);
        ~Sphere();
        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(AABB box);
    private:
        unsigned int radius;
        
};