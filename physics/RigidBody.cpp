#include <limits>

#include "RigidBody.hpp"
#include "Collider.hpp"

RigidBody::RigidBody()
{

}

glm::mat4 RigidBody::GetTransform()
{
    return this->transform;
}

void RigidBody::Integrate(float deltaTime)
{
    // All the magic happens here
}

void RigidBody::SetMass(float mass)
{
    assert(mass != 0);
    this->SetInverseMass(1.f/mass);
}

float RigidBody::GetMass()
{
    if (this->inverseMass == 0.f)
        return std::numeric_limits<float>::max();
    else
        return 1.f/this->inverseMass;
}

void RigidBody::SetInverseMass(float inverseMass)
{
    this->inverseMass = inverseMass;
}

float RigidBody::GetInverseMass()
{
    return this->inverseMass;
}

bool RigidBody::HasFiniteMass()
{
    return this->inverseMass > 0.f;
}

void RigidBody::SetInertiaTensor(glm::mat3 inertiaTensor)
{
    this->inverseInertiaTensor = glm::inverse(inertiaTensor);
}

glm::mat3 RigidBody::GetInertiaTensor()
{
    return glm::inverse(this->inverseInertiaTensor);
}

glm::mat3 RigidBody::GetInertiaTensorWorld()
{
    return glm::inverse(this->inverseInertiaTensorWorld);
}

void RigidBody::SetInverseInertiaTensor(glm::mat3 inverseInertiaTensor)
{
    this->inverseInertiaTensor = inverseInertiaTensor;
}

glm::mat3 RigidBody::GetInverseInertiaTensor()
{
    return this->inverseInertiaTensor;
}

glm::mat3 RigidBody::GetInverseInertiaTensorWorld()
{
    return this->inverseInertiaTensorWorld;
}

void RigidBody::SetPosition(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 RigidBody::GetPosition()
{
    return this->position;
}

void RigidBody::SetVelocity(glm::vec3 velocity)
{
    this->velocity = velocity;
}

glm::vec3 RigidBody::GetVelocity()
{
    return this->velocity;
}

void RigidBody::SetAcceleration(glm::vec3 acceleration)
{
    this->acceleration = acceleration;
}

glm::vec3 RigidBody::GetAcceleration()
{
    return this->acceleration;
}

void RigidBody::SetOrientation(glm::quat orientation)
{
    this->orientation = orientation;
}

glm::quat RigidBody::GetOrientation()
{
    return this->orientation;
}

void RigidBody::SetAngularVelocity(glm::vec3 angularVelocity)
{
    this->angularVelocity = angularVelocity;
}

glm::vec3 RigidBody::GetAngularVelocity()
{
    return this->angularVelocity;
}

void RigidBody::SetAngularAcceleration(glm::vec3 acceleration)
{
    this->angularAcceleration = acceleration;
}

glm::vec3 RigidBody::GetAngularAcceleration()
{
    return this->angularAcceleration;
}

void RigidBody::AddCollider(std::shared_ptr<Collider> collider)
{
    this->colliders.push_back(collider);
}

const std::vector<std::shared_ptr<Collider>> RigidBody::GetColliders()
{
    return this->colliders;
}