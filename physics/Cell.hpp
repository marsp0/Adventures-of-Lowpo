#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Collider.hpp"

class Cell
{
    public:
        Cell(glm::vec3 center, float halfWidth);
        
        void CheckCollisions();

        // Data
        std::vector<std::shared_ptr<Collider>> objects;
        glm::vec3 center;
        float halfWidth;
    private:


};