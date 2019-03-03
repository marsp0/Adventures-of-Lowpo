#include "Collider.hpp"

Collider::Collider(glm::vec3 center, bool dynamic, ColliderType colliderType) : center(center), dynamic(dynamic), type(colliderType)
{
    
}

Collider::~Collider()
{
    
}