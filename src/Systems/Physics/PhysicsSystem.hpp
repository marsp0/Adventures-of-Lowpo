#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>

#include "Grid.hpp"
#include "../../Entity.hpp"
#include "../Messaging/Message.hpp"
#include "../../Components/PhysicsComponent.hpp"

class PhysicsSystem
{
    public:
        PhysicsSystem(float gridLength, float cellHalfWidth);
        ~PhysicsSystem();

        void Insert(std::vector<std::shared_ptr<Collider>>& colliders);
        void Update(float deltaTime, 
                    std::vector<std::shared_ptr<Entity>>& entities,
                    std::vector<Message>& messages,
                    std::vector<Message>& globalQueue);
        /**
        Method that iterates over all the collisions and resolves them one by one.
        Impulse based formula is used here. Check
        https://www.scss.tcd.ie/~manzkem/CS7057/cs7057-1516-09-CollisionResponse-mm.pdf
        https://en.wikipedia.org/wiki/Collision_response#Impulse-based_reaction_model
        */
        void Solve(	std::vector<std::shared_ptr<Entity>>& entities, 
        			std::vector<std::shared_ptr<Collision>>& collisions, 
        			std::unordered_map<int, int>& idToIndexMap);

        void HandleMessages(std::vector<Message>& messages, PhysicsComponent* component);

    private:

        Grid                grid;
        std::uint32_t       primaryBitset;
};