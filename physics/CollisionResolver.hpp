#pragma once

#include <glm/glm.hpp>

#include "Collision.hpp"

class CollisionResolver
{
    public:

        CollisionResolver();

        void Solve(std::vector<std::shared_ptr<Collision>>& collisions);
    private:
};