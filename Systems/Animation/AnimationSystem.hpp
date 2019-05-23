#pragma once

#include "Components/Component.hpp"
#include <Entity.hpp>

class AnimationSystem
{
    public:
        AnimationSystem();
        ~AnimationSystem();

        void Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

    private:
        std::uint32_t requiredBitset;
};