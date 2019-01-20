#include "BoneAnimation.hpp"

BoneAnimation::BoneAnimation(std::string name) : 
        name(name)
{

}

void BoneAnimation::AddKeyframe(Keyframe frame)
{
    this->keyframes.push_back(frame);
}

std::string BoneAnimation::GetBoneName()
{
    return this->name;
}

Transform BoneAnimation::GetTransformAtTick(float tick)
{   
    Transform transform;
    
    Keyframe frame1 = this->keyframes[0];
    Keyframe frame2 = this->keyframes[0];
    for (auto i = this->keyframes.begin() + 1; i != this->keyframes.end(); ++i)
    {
        frame1 = frame2;
        frame2 = *i;
        if (i->GetStartTime() > tick)
        {
            break;
        }
    }
    Keyframe interpolatedFrame = frame1.Interpolate(frame1, frame2, tick);
    transform.SetScale(interpolatedFrame.GetScale());
    transform.SetPosition(interpolatedFrame.GetPosition());
    transform.SetRotation(interpolatedFrame.GetRotation());
    return transform;
}