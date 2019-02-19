#pragma once

#include <glm/glm.hpp>

#include "Collider.hpp"
#include "BoundingBox.hpp"
#include "Sphere.hpp"

class Triangle : public Collider
{
    public:
        Triangle(glm::vec3 center, glm::vec3 normal, bool dynamic);
        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(BoundingBox box);
    private:
        glm::vec3 normal;
};