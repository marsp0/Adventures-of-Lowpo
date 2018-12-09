#include "Keyframe.hpp"

Keyframe::Keyframe(const glm::vec3& translation, glm::quat& rotation, glm::vec3& scale, float startTime):
    translation(translation), rotation(rotation), scale(scale), startTime(startTime)
{

}

float Keyframe::GetStartTime()
{
    return this->startTime;
}

glm::vec3 Keyframe::GetScale()
{
    return this->scale;
}

glm::vec3 Keyframe::GetPosition()
{
    return this->translation;
}

glm::quat Keyframe::GetRotation()
{
    return this->rotation;
}

Keyframe Keyframe::Interpolate(Keyframe& frame1, Keyframe& frame2, float time)
{
    // interpolate scale
    // interpolate translation
    // interpolate rotation

    float dt = frame2.startTime - frame1.startTime;
    float t = (time - frame1.startTime)/dt;

    glm::vec3 interpolatedTranslation = (1-t) * frame1.GetPosition() + t * frame2.GetPosition();
    glm::vec3 interpolatedScale = (1-t) * frame1.GetScale() + t * frame2.GetScale();
    glm::quat interpolatedRotation = glm::normalize(glm::slerp(frame1.GetRotation(), frame2.GetRotation(), t));
    return Keyframe(interpolatedTranslation, interpolatedRotation, interpolatedScale , time);
}