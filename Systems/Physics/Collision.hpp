#pragma once

#include <memory>
#include <vector>

#include "Contact.hpp"
#include "Collider.hpp"

class PhysicsComponent;
/* 
Collision represents a collision between 2 colliders. It contains one or more contacts
 */
class Collision
{
    public:
        Collision(std::shared_ptr<PhysicsComponent> first,
                    std::shared_ptr<Collider> firstCollider,
                    std::shared_ptr<PhysicsComponent> second,
                    std::shared_ptr<Collider> secondCollider,
                    std::vector<Contact> contacts);
        std::shared_ptr<PhysicsComponent> first;
        std::shared_ptr<Collider> firstCollider;
        std::shared_ptr<PhysicsComponent> second;
        std::shared_ptr<Collider> secondCollider;
        std::vector<Contact> contacts;
    private:
};