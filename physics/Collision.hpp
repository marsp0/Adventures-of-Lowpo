#pragma once

#include <memory>
#include <vector>

#include "Collider.hpp"
#include "Contact.hpp"

/* 
Collision represents a collision between 2 colliders. It contains one or more contacts
 */
class Collision
{
    public:
        Collision(std::shared_ptr<Collider> first,
                    std::shared_ptr<Collider> second,
                    std::vector<Contact> contacts);
        std::shared_ptr<Collider> first;
        std::shared_ptr<Collider> second;
        std::vector<Contact> contacts;
    private:
};