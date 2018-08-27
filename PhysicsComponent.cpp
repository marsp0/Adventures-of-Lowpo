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
    std::cout << this->position.x << std::endl;
    std::cout << this->position.y << std::endl;
    std::cout << this->position.z << std::endl;
    std::cout << "vel is" << std::endl;
    std::cout << this->velocity.x << std::endl;
    std::cout << this->velocity.y << std::endl;
    std::cout << this->velocity.z << std::endl;
    std::cout << std::endl;
}


void PhysicsComponent::SetVelocity(glm::vec3 velocity)
{
    std::cout << this->velocity.x << std::endl;
    this->velocity = velocity;
    std::cout << this->velocity.x << std::endl;
}

glm::vec3 PhysicsComponent::GetVelocity()
{
    return this->velocity;
}

glm::vec3 PhysicsComponent::GetPosition()
{
    return this->position;
}
