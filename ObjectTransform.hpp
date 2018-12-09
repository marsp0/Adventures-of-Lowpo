#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <iostream>


class Transform
{
    public:
        Transform();

        glm::mat4   GetWorldMatrix();
        glm::mat4   GetMatrix();
        
        void        SetPosition(glm::vec3 position);
        
        // TODO : Add Euler angles and axis-angle rotations
        void        SetRotation(glm::quat rotation);
        glm::quat   GetRotation();
        void        Rotate(glm::quat rotation);

        void        SetScale(glm::vec3 scale);
        glm::vec3   GetScale();
        void        Scale(glm::vec3 scale);

    private:

        glm::vec3 position;
        glm::vec3 scale;
        glm::quat rotation;
};