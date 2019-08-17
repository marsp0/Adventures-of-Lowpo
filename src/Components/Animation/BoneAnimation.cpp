#include "BoneAnimation.hpp"

BoneAnimation::BoneAnimation(std::string name, int boneIndex) : 
        name(name),
        boneIndex(boneIndex)
{

}

glm::mat4 BoneAnimation::GetTransformAtTick(float tick)
{
    int first = 0;
    int second = 0;
    for (int i = 0; i < this->startTimes.size(); i++)
    {
        first = second;
        second = i;
        if (this->startTimes[i] > tick)
            break;
    }
    // interpolate between the two frames.
    // length of frame
    float startTimeA = this->startTimes[first];
    float startTimeB = this->startTimes[second];
    
    glm::vec3 translationA = this->translations[first];
    glm::vec3 translationB = this->translations[second];
    
    glm::vec3 scaleA = this->scales[first];
    glm::vec3 scaleB = this->scales[second];
    
    glm::quat rotationA = this->rotations[first];
    glm::quat rotationB = this->rotations[second];

    float dt = this->startTimes[second] - this->startTimes[first];
    // length of time we have passed from the start of frame
    float t = (tick - this->startTimes[first])/dt;
    glm::vec3 interpT = (1 - t) * translationA + t * translationB;
    glm::vec3 interpS = (1 - t) * scaleA + t * scaleB;
    glm::quat interpR = glm::normalize(glm::slerp(rotationA, rotationB, t));

    glm::mat4 scale = glm::mat4(
        interpS.x,          0,          0, 0,
                0,  interpS.y,          0, 0,
                0,          0,  interpS.z, 0,
                0,          0,          0, 1
    );
    glm::mat4 translation = glm::mat4(
        interpS.x,          0,          0, 0,
                0,  interpS.y,          0, 0,
                0,          0,  interpS.z, 0,
        interpT.x,  interpT.y,  interpT.z, 1
    );
    glm::mat4 result(1.0f);
    glm::mat4 rotationMat = glm::mat4_cast(interpR);
    result *=  translation * rotationMat * scale;
    return result;
}