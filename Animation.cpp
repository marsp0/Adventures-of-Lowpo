#include "Animation.hpp"

Animation::Animation(std::string name, float tickDuration, 
                        float ticksPerSecond, 
                        float duration, 
                        std::vector<std::shared_ptr<BoneAnimation>> boneAnimations) : 
                        name(name), boneAnimations(boneAnimations), 
                        ticksPerSecond(ticksPerSecond), tickDuration(tickDuration),
                        duration(duration)
{
}

float Animation::GetTickForTime(float time)
{
    return fmod(time * this->ticksPerSecond , this->duration);
}

std::string Animation::GetAnimationName()
{
    return this->name;
}

std::vector<std::shared_ptr<BoneAnimation>>& Animation::GetAnimationData()
{
    return this->boneAnimations;
}