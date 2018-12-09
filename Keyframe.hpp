#pragma once

#include <glm/gtc/quaternion.hpp>

class Keyframe
{
    public:
        
        Keyframe(const glm::vec3& translation, glm::quat& rotation, glm::vec3& scale, float startTime);

        float       GetStartTime();
        glm::vec3   GetScale();
        glm::vec3   GetPosition();
        glm::quat   GetRotation();

        Keyframe    Interpolate(Keyframe& frame1, Keyframe& frame2, float time);

    private:

        glm::vec3   translation;
        glm::quat   rotation;
        glm::vec3   scale;
        float       startTime;
};