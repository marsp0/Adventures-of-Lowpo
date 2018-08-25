#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <iostream>

// #include "Terrain.hpp"
// #include "GameObject.hpp"

class Terrain;
class GameObject;
class Transform
{
    public:
        Transform(GameObject* parent);
        Transform(Terrain* parent);
        // Transform(const Transform& transform);

        glm::mat4   getWorldMatrix();
        glm::vec3   GetPosition();

        // TODO : Add Euler angles and axis-angle rotations
        void        SetRotation(glm::quat rotation);
        glm::quat   GetRotation();
        void        Rotate(glm::quat rotation);

        void        SetScale(glm::vec3 scale);
        glm::vec3   GetScale();
        void        Scale(glm::vec3 scale);

    private:

        glm::vec3 scale;
        glm::quat rotation;
        GameObject* parentGameObject;
        Terrain* parentTerrain;
};