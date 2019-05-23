#pragma once

#include "Collider.hpp"

class AABB : public Collider
{
    public:

        AABB( glm::vec3 center, 
              std::string name,
              glm::vec3 axisRadii, 
              ColliderType colliderType, 
              std::shared_ptr<PhysicsComponent> component);
        ~AABB();
        virtual void ComputeDerivedData();
        glm::vec3 axisRadii;

    private:
};