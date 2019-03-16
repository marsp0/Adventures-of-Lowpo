#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class RigidBody
{
    public:

        RigidBody();

        glm::mat4 GetTransform();

        void Integrate(float deltaTime);

        void SetMass(float mass);
        float GetMass();

        void SetInverseMass(float inverseMass);
        float GetInverseMass();

        bool HasFiniteMass();

        void SetInertiaTensor(glm::mat3 inertiaTensor);
        glm::mat3 GetInertiaTensor();

        glm::mat3 GetInertiaTensorWorld();
        glm::mat3 GetInverseInertiaTensorWorld();

        void SetInverseInertiaTensor(glm::mat3 inverseInertiaTensor);
        glm::mat3 GetInverseInertiaTensor();

        void SetPosition(glm::vec3 position);
        glm::vec3 GetPosition();

        void SetVelocity(glm::vec3 velocity);
        glm::vec3 GetVelocity();

        void SetAcceleration(glm::vec3 acceleration);
        glm::vec3 GetAcceleration();

        void SetOrientation(glm::quat orientation);
        glm::quat GetOrientation();

        void SetAngularVelocity(glm::vec3 angularVelocity);
        glm::vec3 GetAngularVelocity();

    private:
    
        float inverseMass;
        glm::mat3 inverseInertiaTensor;
        glm::mat3 inverseInertiaTensorWorld;
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 lastFrameAcceleration;
        glm::quat orientation;
        // angular velocity;
        glm::vec3 angularVelocity;

        glm::mat4 transform;

        glm::vec3 forceAccumulator;
        glm::vec3 torqueAccumulator;
};