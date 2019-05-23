#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Grid.hpp"
#include "Entity.hpp"
#include "CollisionResolver.hpp"

class PhysicsSystem
{
    public:
        PhysicsSystem(float gridLength, float cellHalfWidth);
        ~PhysicsSystem();

        void Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

    private:

        Grid                grid;
        std::uint32_t       requiredBitset;
        CollisionResolver   collisionResolver;
};