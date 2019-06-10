#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>

#include "Grid.hpp"
#include "../../Entity.hpp"

class PhysicsSystem
{
    public:
        PhysicsSystem(float gridLength, float cellHalfWidth);
        ~PhysicsSystem();

        void Insert(std::vector<std::shared_ptr<Collider>>& colliders);
        void Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

        void Solve(	std::vector<std::shared_ptr<Entity>>& entities, 
        			std::vector<std::shared_ptr<Collision>>& collisions, 
        			std::unordered_map<int, int>& idToIndexMap);

    private:

        Grid                grid;
        std::uint32_t       primaryBitset;
};