#include "Animation.hpp"

Animation::Animation()
{

}

float Animation::GetTickForTime(float time)
{
    return fmod(time * this->ticksPerSecond, tickDuration);
}

std::string Animation::GetAnimationName()
{
    return this->name;
}

std::vector<std::shared_ptr<BoneAnimation>>& Animation::GetAnimationData()
{
    return this->boneAnimations;
}