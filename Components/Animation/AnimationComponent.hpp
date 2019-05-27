#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Component.hpp"
#include "Animation.hpp"
#include "Bone.hpp"

class AnimationComponent : public Component
{
    public:

        AnimationComponent();
        ~AnimationComponent();

        Animation& GetCurrentAnimation();
        Bone& GetBone(int index);

        // -1 means that no animation is currently active
        int current;
        float animationTime;
        float speedMultiplier;
        std::vector<Bone> bones;
        std::vector<Animation> animations;
        std::unordered_map<int, int> actionToAnimation;
    
    private:
};