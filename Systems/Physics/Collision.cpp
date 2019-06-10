#include "Collision.hpp"


Collision::Collision(  int firstID,
                    std::shared_ptr<Collider> firstCollider,
                    int secondID,
                    std::shared_ptr<Collider> secondCollider,
                    std::vector<Contact> contacts): \
                    first(firstID), 
                    firstCollider(firstCollider),
                    second(secondID),
                    secondCollider(secondCollider),
                    contacts(contacts)
{

}