#pragma once

#include <glm/glm.hpp>

#include "AABB.hpp"

class PhysicsObject
{
    public:
        PhysicsObject();
        void Integrate(float deltaTime);
        const AABB GetBoundingBox();
    private:
        glm::vec3 position;
        glm::vec3 velocity;
        AABB boundingBox;
};