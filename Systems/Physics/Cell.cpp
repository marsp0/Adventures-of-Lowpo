#include <iostream>

#include "Cell.hpp"
#include "CollisionDetector.hpp"
#include "PhysicsComponent.hpp"

Cell::Cell(glm::vec3 center, float halfWidth) : center(center), halfWidth(halfWidth)
{
}

void Cell::Insert(std::shared_ptr<Collider> object)
{
    if (object->GetType() == DynamicType::Dynamic || object->GetType() == DynamicType::WithPhysics)
        this->dynamicColliders.push_back(object);
    else
        this->staticColliders.push_back(object);
}

void Cell::Remove(std::shared_ptr<Collider> object)
{
    if (object->GetType() == DynamicType::Dynamic || object->GetType() == DynamicType::WithPhysics)
    {
        for (int i = 0; i < this->dynamicColliders.size(); i++)
        {
            if (this->dynamicColliders[i] == object)
            {
                std::vector<std::shared_ptr<Collider>>::iterator it = this->dynamicColliders.begin() + i;
                this->dynamicColliders.erase(it);
            }
        }
    }
    else
    {
        for (int i = 0; i < this->staticColliders.size(); i++)
        {
            if (this->staticColliders[i] == object)
            {
                std::vector<std::shared_ptr<Collider>>::iterator it = this->staticColliders.begin() + i;
                this->staticColliders.erase(it);
            }
        }
    }
}

const std::vector<std::shared_ptr<Collider>>& Cell::GetDynamicColliders()
{
    return this->dynamicColliders;
}

const std::vector<std::shared_ptr<Collider>>& Cell::GetStaticColliders()
{
    return this->staticColliders;
}