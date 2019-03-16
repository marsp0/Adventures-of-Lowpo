#include "Collision.hpp"


Collision::Collision(std::shared_ptr<Collider> first, std::shared_ptr<Collider> second, std::vector<Contact> contacts) :
first(first), second(second), contacts(contacts)
{

}