#include "Cell.hpp"

#include <iostream>

#include "CollisionDetector.hpp"

Cell::Cell(glm::vec3 center, float halfWidth) : center(center), halfWidth(halfWidth)
{
}

void Cell::CheckCollisions(std::shared_ptr<CollisionDetector> detector)
{
    // Environment collisions
    for (int i = 0; i < this->dynamicObjects.size(); i++)
    {
        for (int j = 0; j < this->staticObjects.size(); j++)
        {
            std::shared_ptr<Collider> first = this->dynamicObjects[i];
            std::shared_ptr<Collider> second = this->staticObjects[j];
            if (detector->CheckCollision(first,second))
            {
                std::cout << "Found one" << std::endl;
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
            if (detector->CheckCollision(first,second))
            {
                std::cout << "Found one" << std::endl;
            }
        }
    }
}

void Cell::Insert(std::shared_ptr<Collider> object)
{
    if (object->dynamicType == DynamicType::DYNAMIC || object->dynamicType == DynamicType::DYNAMIC_LIVE)
        this->dynamicObjects.push_back(object);
    else
        this->staticObjects.push_back(object);
}

void Cell::Remove(std::shared_ptr<Collider> object)
{
    if (object->dynamicType == DynamicType::DYNAMIC || object->dynamicType == DynamicType::DYNAMIC_LIVE)
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