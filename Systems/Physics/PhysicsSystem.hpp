#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Grid.hpp"
#include "../../Entity.hpp"
#include "CollisionResolver.hpp"

class PhysicsSystem
{
    public:
        PhysicsSystem(float gridLength, float cellHalfWidth);
        ~PhysicsSystem();

        void Init(std::vector<std::shared_ptr<Collider>>& colliders);
        void Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

    private:

        Grid                grid;
        std::uint32_t       primaryBitset;
        CollisionResolver   collisionResolver;
};