#pragma once

#include <glm/glm.hpp>

class Camera
{
    public:
        
        Camera( glm::vec3   position,
                glm::vec3   direction,
                float       aspectRatio);

        glm::mat4   GetViewMatrix();
        glm::vec3   GetCameraPosition();
        glm::mat4   GetProjectionMatrix();
        void        Update(glm::vec3 position);
        
        
        glm::vec3   direction;
        glm::vec3   up;
        glm::vec3   right;
        glm::vec3   worldUp;
        glm::vec3   position;

        double      lastX;
        double      lastY;
        
        float       aspectRatio;
        
    private:
        

        float       yaw;
        float       pitch;
        float       sensitivity;
        glm::mat4   projectionMatrix;
        unsigned int radius;
};