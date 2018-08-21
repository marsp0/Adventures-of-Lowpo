#pragma once

#include <glm/glm.hpp>

#include "AABB.hpp"

class PhysicsComponent
{
    public:
        PhysicsComponent(glm::vec3 min, glm::vec3 max, glm::vec3 velocity);
        void Integrate(float deltaTime);
        const AABB GetBoundingBox();

        glm::vec3   position;
        glm::vec3   velocity;

    private:
        
        AABB        boundingBox;
};