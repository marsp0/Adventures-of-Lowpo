#pragma once

#include "BoundingVolume.hpp"
#include "BoundingBox.hpp"
#include "Triangle.hpp"

class Sphere : public BoundingVolume
{
    public:
        Sphere(glm::vec3 center, float radius);
        ~Sphere();
        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(BoundingBox box);
    private:
        unsigned int radius;
        
};