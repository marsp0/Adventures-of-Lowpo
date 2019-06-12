#pragma once

#include <vector>
#include <memory>
#include "../../Components/Component.hpp"
#include "../Events/Event.hpp"

class Entity;
class AnimationSystem
{
    public:
        AnimationSystem();
        ~AnimationSystem();

        void Update(float deltaTime, 
        			std::vector<std::shared_ptr<Entity>>& entities,
        			std::vector<Event>& events,
        			std::vector<Event>& globalQueue);

    private:
        std::uint32_t primaryBitset;
};