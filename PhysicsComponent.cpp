#include <iostream>
#include "PhysicsComponent.hpp"

PhysicsComponent::PhysicsComponent(glm::vec3 min, glm::vec3 max, glm::vec3 velocity) :
    boundingBox(min,max)
{
    this->position = glm::vec3((max.x - min.x)/2 ,(max.y - min.y)/2, (max.z - min.z)/2);
    this->velocity = velocity;
    std::cout << "done with the physics component initialization" << std::endl;
}

void PhysicsComponent::Integrate(float deltaTime)
{
    this->position += this->velocity * deltaTime;
}