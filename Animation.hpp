#pragma once

#include <vector>
#include "BoneAnimation.hpp"

class Animation
{
    public:
        Animation();

        float                       GetTickForTime(float time);
        std::string                 GetAnimationName();
        std::vector<std::shared_ptr<BoneAnimation>>& GetAnimationData();

    private:

        std::string name;
        // The duration of the animation in ticks.
        float tickDuration;
        float ticksPerSecond;
        std::vector<std::shared_ptr<BoneAnimation>> boneAnimations;
};