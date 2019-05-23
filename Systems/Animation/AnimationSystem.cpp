#include "AnimationSystem.hpp"

#include "Components/Animation/AnimationComponent.hpp"

AnimationSystem::AnimationSystem()
{
    this->requiredBitset = ComponentType::Animated;
}

AnimationSystem::~AnimationSystem()
{

}

void AnimationSystem::Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities)
{
    // TODO : figure out how the triggering, stopping etc is going to work.
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->requiredBitset))
        {
            // 1. update local animations using the bone animations
            AnimationComponent component = entities[i]->GetComponent<AnimationComponent>(ComponentType::Animated);
            Animation animation = component.GetCurrentAnimation();
            component.animationTime += component.speedMultiplier * deltaTime;
            float animCurrentTime = animation.GetTickForTime(component.animationTime);
            for (int j = 0; j < animation.boneAnimations.size(); j++)
            {
                BoneAnimation boneAnimation = animation.GetBoneAnimation(j);
                glm::mat4 localAnimTransform = boneAnimation.GetTransformAtTick(animCurrentTime);
                int boneIndex = boneAnimation.boneIndex;
                Bone& bone = component.GetBone(boneIndex);
                bone.localAnimationTransform = localAnimTransform;
            }
            // 2. update bones
            for (int j = 1; j < component.bones.size() ; j++)
            {
                Bone& bone = component.GetBone(j);
                Bone& parent = component.GetBone(bone.parentIndex);
                bone.animationTransform = parent.animationTransform * bone.localAnimationTransform;
            }
        }
    }
}