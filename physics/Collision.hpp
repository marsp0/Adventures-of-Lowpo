#pragma once

#include <memory>
#include <vector>

#include "Contact.hpp"
#include "Collider.hpp"
#include "RigidBody.hpp"

/* 
Collision represents a collision between 2 colliders. It contains one or more contacts
 */
class Collision
{
    public:
        Collision(std::shared_ptr<RigidBody> first,
                    std::shared_ptr<Collider> firstCollider,
                    std::shared_ptr<RigidBody> second,
                    std::shared_ptr<Collider> secondCollider,
                    std::vector<Contact> contacts);
        std::shared_ptr<RigidBody> first;
        std::shared_ptr<Collider> firstCollider;
        std::shared_ptr<RigidBody> second;
        std::shared_ptr<Collider> secondCollider;
        std::vector<Contact> contacts;
    private:
};