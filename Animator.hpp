#pragma once

#include <unordered_map>
#include <memory>
#include "Skeleton.hpp"
#include "Animation.hpp"

class Animator
{
    public:
        Animator();
        void Update(float deltaTime);
        void AddAnimation(std::shared_ptr<Animation>& animation);
        void SetAnimation(std::string& animationName);
        void SetAnimationTime(float time);
        void SetAnimationSpeed(float speedMultiply);

    private:

        // animation time - the time that has passed since the start of the animation
        float                                       animationTime;
        float                                       speedMultiply;
        std::unique_ptr<Skeleton>                   skeleton;
        std::shared_ptr<Animation>                  currentAnimation;
        std::unordered_map<std::string , std::shared_ptr<Animation>> animations;
};