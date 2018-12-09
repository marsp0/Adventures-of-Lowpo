#include "BoneAnimation.hpp"

BoneAnimation::BoneAnimation(std::string name) : 
        name(name)
{

}

void BoneAnimation::AddKeyframe(Keyframe frame)
{
    if (this->keyframes.empty())
    {
        this->keyframes.push_back(frame);
    }
    else
    {
        auto i = this->keyframes.end();
        for (; i != this->keyframes.begin() ; i--)
        {
            // if the start time of the previous keyframe is smaller than
            // the current one, then we have found the spot to insert
            if ((i-1)->GetStartTime() < (i)->GetStartTime())
            {
                break;
            }
        }
        // linear time but the keyframes should not be a lot
        this->keyframes.insert(i,frame);
    }
}

std::string BoneAnimation::GetBoneName()
{
    return this->name;
}

Transform BoneAnimation::GetTransformAtTick(float tick)
{
    Transform transform;
    if (this->keyframes.size() == 0)
    {
        transform.SetPosition(this->keyframes[0].GetPosition());
        transform.SetRotation(this->keyframes[0].GetRotation());
        transform.SetScale(this->keyframes[0].GetScale());
        return transform;
    }
    else
    {
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
}