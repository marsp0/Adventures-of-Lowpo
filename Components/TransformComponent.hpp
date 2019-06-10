#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.hpp"

class TransformComponent : public Component
{
    public:
        TransformComponent(glm::vec3 position, glm::quat orientation);
        ~TransformComponent();

        glm::mat4 GetWorldTransform();
        
        glm::vec3 position;
        glm::quat orientation;
};