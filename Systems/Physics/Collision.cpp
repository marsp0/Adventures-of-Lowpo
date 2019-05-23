#include "Collision.hpp"


Collision::Collision(std::shared_ptr<PhysicsComponent> first,
                    std::shared_ptr<Collider> firstCollider,
                    std::shared_ptr<PhysicsComponent> second,
                    std::shared_ptr<Collider> secondCollider,
                    std::vector<Contact> contacts):
                    first(first), 
                    firstCollider(firstCollider),
                    second(second),
                    secondCollider(secondCollider),
                    contacts(contacts)
{

}