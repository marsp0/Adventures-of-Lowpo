#include <limits>

#include "RigidBody.hpp"

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
    return this->inverseMass >= 0.f;
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

void RigidBody::SetOrientation(glm::quat orientation)
{
    this->orientation = orientation;
}

glm::quat RigidBody::GetOrientation()
{
    return this->orientation;
}
