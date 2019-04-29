#pragma once

#include "Collider.hpp"

class AABB : public Collider
{
    public:

        AABB( glm::vec3 center, 
              std::string name,
              glm::vec3 axisRadii, 
              ColliderType colliderType, 
              DynamicType dynamicType, 
              std::shared_ptr<RigidBody> rigidBody);
        ~AABB();
        virtual void ComputeDerivedData();
        glm::vec3 axisRadii;

    private:
};