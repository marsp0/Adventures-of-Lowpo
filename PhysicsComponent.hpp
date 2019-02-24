#pragma once

#include <glm/glm.hpp>

#include "BoundingBox.hpp"

class PhysicsComponent
{
    public:

        // Methods
        PhysicsComponent(glm::vec3 min, glm::vec3 max, glm::vec3 velocity, ObjectType type);
        void        Integrate(float deltaTime);
        const BoundingBox  GetBoundingBox();

        void        SetVelocity(glm::vec3 velocity);
        glm::vec3   GetVelocity();
        glm::vec3   GetPosition();

        // DATA
        glm::vec3   position;
        glm::vec3   velocity;
        BoundingBox        boundingBox;
        
};