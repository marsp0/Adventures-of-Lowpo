#include "Animator.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Animator::Animator(std::shared_ptr<BoneTreeNode> root, glm::mat4 rootInverseTransform):
        speedMultiply(.03f), animationTime(0)
{
    this->skeleton = std::make_unique<Skeleton>(root, rootInverseTransform);
}

void Animator::Update(float deltaTime)
{
    // Get the current keyframe by interpolating between the closest ones.
    this->animationTime += this->speedMultiply * deltaTime;
    float tick = this->currentAnimation->GetTickForTime(this->animationTime);
    std::vector<std::shared_ptr<BoneAnimation>>& boneAnimations = this->currentAnimation->GetAnimationData();
    for (int i = 0; i < boneAnimations.size() ; i++)
    {
        Transform transform = boneAnimations[i]->GetTransformAtTick(tick);
        std::string boneName = boneAnimations[i]->GetBoneName();
        std::shared_ptr<Bone> bone = this->skeleton->GetBone(boneName);
        bone->SetLocalAnimationTransform(transform.GetMatrix());
    }
    // Update the tree structure
    this->skeleton->UpdateBones();
}

void Animator::AddAnimation(std::shared_ptr<Animation> animation)
{
    this->animations[animation->GetAnimationName()] = animation;
}

void Animator::SetAnimation(std::string animationName)
{
    this->currentAnimation = this->animations[animationName];
}

void Animator::SetAnimationTime(float time)
{
    this->animationTime = time;
}

void Animator::SetAnimationSpeed(float speedMultiply)
{
    this->speedMultiply = speedMultiply;
}