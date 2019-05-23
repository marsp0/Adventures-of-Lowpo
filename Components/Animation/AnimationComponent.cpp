#include "AnimationComponent.hpp"

AnimationComponent::AnimationComponent() : Component(ComponentType::Animated)
{

}

AnimationComponent::~AnimationComponent()
{

}

Animation& AnimationComponent::GetCurrentAnimation()
{
    return this->animations[current];
}

Bone& AnimationComponent::GetBone(int index)
{
    return this->bones[index];
}