#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Collider.hpp"
#include "AABB.hpp"
#include "Sphere.hpp"

class Triangle : public Collider
{
    public:
        Triangle(   glm::vec3               center, 
                    glm::vec3               normal, 
                    std::vector<glm::vec3>  points, 
                    bool                    dynamic, 
                    ColliderType            colliderType);
        bool CheckCollision(std::shared_ptr<Collider> collider);
        bool CheckCollision(std::shared_ptr<Triangle> triangle);
        bool CheckCollision(std::shared_ptr<Sphere> sphere);
        bool CheckCollision(std::shared_ptr<AABB> box);

        glm::vec3 normal;
        std::vector<glm::vec3> points;
    private:
};