#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Collider.hpp"
#include "AABB.hpp"
#include "Sphere.hpp"

class Triangle : public Collider
{
    public:
        Triangle(glm::vec3 center, glm::vec3 normal, std::vector<glm::vec3> points, bool dynamic);
        bool CheckCollision(Triangle triangle);
        bool CheckCollision(Sphere sphere);
        bool CheckCollision(AABB box);

        glm::vec3 normal;
        std::vector<glm::vec3> points;
    private:
};