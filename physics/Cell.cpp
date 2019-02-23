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
    // for (int i = 0; i < this->objects.size(); i++)
    // {
    //     for (int j = i; j < this->objects.size(); j++)
    //     {
    //         // test here collisions
    //         std::cout << "Checking " << i << " and " << j << std::endl;
    //     }
    // }
    for (int i = 0; i < this->objects.size(); i++)
    {
        float distanceX = abs(this->objects[i]->center.x - this->center.x);
        float distanceZ = abs(this->objects[i]->center.z - this->center.z);
        if (distanceX > halfWidth || distanceZ > halfWidth)
        {
            std::cout << "WRONG" << std::endl;
            std::cout << distanceX << std::endl;
            std::cout << distanceZ << std::endl;
            std::cout << std::endl;
        }
    }
    if (this->objects.size() > 0)
    {
        std::cout << this->center.x << std::endl;
        std::cout << this->center.z << std::endl;
        std::cout << this->objects.size() << std::endl;
    }
    
}