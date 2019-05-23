#include "Animation.hpp"

Animation::Animation(   std::string name, 
                        float tickDuration, 
                        float ticksPerSecond, 
                        float duration, 
                        std::vector<BoneAnimation> boneAnimations) : 
                        name(name), 
                        boneAnimations(boneAnimations), 
                        ticksPerSecond(ticksPerSecond), 
                        tickDuration(tickDuration),
                        duration(duration)
{

}

float Animation::GetTickForTime(float time)
{
    return fmod(time * this->ticksPerSecond , this->duration);
}

BoneAnimation& Animation::GetBoneAnimation(int index)
{
    return this->boneAnimations[index];
}