#pragma once

#include <vector>
#include <memory>
#include "../../Components/Component.hpp"
#include "../Messaging/Message.hpp"

class Entity;
class AnimationSystem
{
    public:
        AnimationSystem();
        ~AnimationSystem();

        void Update(float deltaTime, 
        			std::vector<std::shared_ptr<Entity>>& entities,
        			std::vector<Message>& events,
        			std::vector<Message>& globalQueue);

    private:
        std::uint32_t primaryBitset;
};