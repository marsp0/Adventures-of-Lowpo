#include "Cell.hpp"

#include <iostream>

Cell::Cell(glm::vec3 center, float halfWidth) : center(center), halfWidth(halfWidth)
{
    // std::cout << "Cell info" << std::endl;
    // std::cout << center.x << std::endl;
    // std::cout << center.y << std::endl;
    // std::cout << center.z << std::endl;
    // std::cout << "halfwidth " << halfWidth<< std::endl;
    // std::cout << std::endl;
}

void Cell::CheckCollisions()
{
    if (this->objects.size() > 0)
    {
        std::cout << "SIZE " << this->objects.size() << std::endl;
    }
    for (int i = 0; i < this->objects.size(); i++)
    {
        for (int j = i + 1; j < this->objects.size(); j++)
        {
            // test here collisions
            std::cout << "got to here " << std::endl;
            bool t = this->objects[i]->CheckCollision(this->objects[j]);
            if (t)
            {
                std::cout << "found collision" << std::endl;
            }
        }
    }
}