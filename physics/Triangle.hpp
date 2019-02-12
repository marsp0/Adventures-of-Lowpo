#pragma once

#include <glm/glm.hpp>

#include "BoundingVolume.hpp"
#include "BoundingBox.hpp"
#include "Sphere.hpp"

class Triangle : public BoundingVolume
{
    public:
        Triangle(glm::vec3 center);
        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(BoundingBox box);
    private:
        glm::vec3 normal;
};