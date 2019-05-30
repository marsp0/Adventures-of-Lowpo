#pragma once

#include <vector>
#include <memory>
#include "../../Components/Component.hpp"

class Entity;
class AnimationSystem
{
    public:
        AnimationSystem();
        ~AnimationSystem();

        void Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

    private:
        std::uint32_t primaryBitset;
};