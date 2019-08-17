#pragma once

#include "Collider.hpp"

class AABB : public Collider
{
    public:

        AABB( int entityID,
              glm::vec3 center, 
              std::string name,
              glm::vec3 axisRadii, 
              ColliderType colliderType,
              DynamicType  dynamicType);
        ~AABB();
        virtual void ComputeDerivedData();
        glm::vec3 axisRadii;

    private:
};