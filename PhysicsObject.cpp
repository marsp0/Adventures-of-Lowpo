#include "PhysicsObject.hpp"

PhysicsObject::PhysicsObject()
{

}

void PhysicsObject::Integrate(float deltaTime)
{
    this->position += this->velocity * deltaTime;
}