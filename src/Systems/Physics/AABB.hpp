#pragma once

#include "Collider.hpp"

class AABB : public Collider
{
    public:

        AABB( int entityID,
              glm::vec3 center, 
              ColliderType colliderType,
              DynamicType  dynamicType,
              glm::vec3 axisRadii);
        ~AABB();

        virtual void ComputeDerivedData();
        virtual void Update(glm::vec3 translation);

        glm::vec3 axisRadii;

    private:

};