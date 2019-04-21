#include "Cell.hpp"

#include <iostream>

#include "CollisionDetector.hpp"

Cell::Cell(glm::vec3 center, float halfWidth) : center(center), halfWidth(halfWidth)
{
}

std::vector<std::shared_ptr<Collision>> Cell::CheckCollisions(std::shared_ptr<CollisionDetector> detector)
{
    std::vector<std::shared_ptr<Collision>> collisions;
    // Environment collisions
    for (int i = 0; i < this->dynamicObjects.size(); i++)
    {
        for (int j = 0; j < this->staticObjects.size(); j++)
        {
            std::shared_ptr<Collider> first = this->dynamicObjects[i];
            std::shared_ptr<Collider> second = this->staticObjects[j];
            std::shared_ptr<Collision> collision = detector->CheckCollision(first,second);
            if (collision != nullptr)
            {
                collisions.push_back(collision);
            }
        }
    }
    // dynamic objects collisions
    for (int i = 0; i < this->dynamicObjects.size() ; i++)
    {
        for (int j = i + 1; j < this->dynamicObjects.size(); j++)
        {
            std::shared_ptr<Collider> first = this->dynamicObjects[i];
            std::shared_ptr<Collider> second = this->dynamicObjects[j];
            std::shared_ptr<Collision> collision = detector->CheckCollision(first,second);
            if (collision != nullptr)
            {
                collisions.push_back(collision);
            }
        }
    }
    return collisions;
}

void Cell::Insert(std::shared_ptr<Collider> object)
{
    if (object->dynamicType == DynamicType::Dynamic || object->dynamicType == DynamicType::WithPhysics)
        this->dynamicObjects.push_back(object);
    else
        this->staticObjects.push_back(object);
}

void Cell::Remove(std::shared_ptr<Collider> object)
{
    if (object->dynamicType == DynamicType::Dynamic || object->dynamicType == DynamicType::WithPhysics)
    {
        for (int i = 0; i < this->dynamicObjects.size(); i++)
        {
            if (this->dynamicObjects[i] == object)
            {
                std::vector<std::shared_ptr<Collider>>::iterator it = this->dynamicObjects.begin() + i;
                this->dynamicObjects.erase(it);
            }
        }
    }
    else
    {
        for (int i = 0; i < this->staticObjects.size(); i++)
        {
            if (this->staticObjects[i] == object)
            {
                std::vector<std::shared_ptr<Collider>>::iterator it = this->staticObjects.begin() + i;
                this->staticObjects.erase(it);
            }
        }
    }
}