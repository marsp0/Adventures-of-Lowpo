#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class BoneAnimation
{
    public:

        BoneAnimation(std::string name, int boneIndex);
        glm::mat4 GetTransformAtTick(float tick);

        // Data member
        int                     boneIndex;
        std::vector<float>      startTimes;
        std::vector<glm::vec3>  scales;
        std::vector<glm::quat>  rotations;
        std::vector<glm::vec3>  translations;
        std::string             name;

    private:
};