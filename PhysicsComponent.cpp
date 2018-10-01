#include <iostream>
#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent(glm::vec3 min, glm::vec3 max, glm::vec3 velocity, ObjectType type) :
    boundingBox(min, max, type)
{
    this->velocity = velocity;
}

void PhysicsComponent::Integrate(float deltaTime)
{
    if (this->boundingBox.type == ObjectType::Dynamic)
    {
        std::cout << "position is " << std::endl;
        std::cout << this->position.x<< std::endl;
        std::cout << this->position.y<< std::endl;
        std::cout << this->position.z<< std::endl;
    }
    this->position += this->velocity * deltaTime;
    this->boundingBox.Update(this->velocity * deltaTime);
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


