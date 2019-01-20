#pragma once

#include <vector>
#include "BoneAnimation.hpp"

class Animation
{
    public:
        Animation(std::string name, float tickDuration, float ticksPerSecond, float duration, std::vector<std::shared_ptr<BoneAnimation>> boneAnimations);

        float                       GetTickForTime(float time);
        std::string                 GetAnimationName();
        std::vector<std::shared_ptr<BoneAnimation>>& GetAnimationData();

    private:

        std::string name;
        // The duration of the animation in ticks.
        float duration;
        float tickDuration;
        float ticksPerSecond;
        std::vector<std::shared_ptr<BoneAnimation>> boneAnimations;
};