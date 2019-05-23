#pragma once

#include <vector>
#include "BoneAnimation.hpp"

class Animation
{
    public:
        Animation(  std::string name, 
                    float tickDuration, 
                    float ticksPerSecond, 
                    float duration, 
                    std::vector<BoneAnimation> boneAnimations);

        float GetTickForTime(float time);
        BoneAnimation& GetBoneAnimation(int index);

        std::string name;
        // The duration of the animation in ticks.
        float duration;
        float tickDuration;
        float ticksPerSecond;
        std::vector<BoneAnimation> boneAnimations;
    
    private:
};