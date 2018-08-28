#include <iostream>
#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent(glm::vec3 min, glm::vec3 max, glm::vec3 velocity) :
    boundingBox(min,max)
{
    this->position = glm::vec3((max.x - min.x)/2 ,(max.y - min.y)/2, (max.z - min.z)/2);
    this->velocity = velocity;
}

void PhysicsComponent::Integrate(float deltaTime)
{
    this->position += this->velocity * deltaTime;
}


void PhysicsComponent::SetVelocity(glm::vec3 velocity)
{
    this->velocity = velocity;
}

glm::vec3 PhysicsComponent::GetVelocity()
{
    return this->velocity;
}

glm::vec3 PhysicsComponent::GetPosition()
{
    return this->position;
}
