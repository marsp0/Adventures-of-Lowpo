#pragma once

#include <vector>
#include <string>
#include "Keyframe.hpp"
#include "ObjectTransform.hpp"

class BoneAnimation
{
    public:

        BoneAnimation(std::string name);
        void                        AddKeyframe(Keyframe frame);
        Transform                   GetTransformAtTick(float tick);
        std::string                 GetBoneName();

    private:

        std::vector<Keyframe>   keyframes;
        std::string             name;
};