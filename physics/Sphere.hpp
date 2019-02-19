#pragma once

#include "Collider.hpp"
#include "BoundingBox.hpp"
#include "Triangle.hpp"

class Sphere : public Collider
{
    public:
        Sphere(glm::vec3 center, float radius, bool dynamic);
        ~Sphere();
        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(BoundingBox box);
    private:
        unsigned int radius;
        
};