#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.hpp"

class TransformComponent : public Component
{
    public:
        TransformComponent();
        ~TransformComponent();

        void SetPosition(glm::vec3 position);
        void SetOrientation(glm::quat orientation);

        glm::vec3 GetPosition();
        glm::quat GetOrientation();
        glm::mat4 GetWorldTransform();

    private:

        glm::vec3 position;
        glm::quat orientation;
};