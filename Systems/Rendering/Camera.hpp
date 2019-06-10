#pragma once

#include <glm/glm.hpp>

class Camera
{
    public:
        
        Camera( glm::vec3   position,
                glm::vec3   direction,
                float       aspectRatio
                glm::vec3   worldUp     = glm::vec3(0.0f,1.0f,0.0f),
                float       yaw         = 90.0f,
                float       pitch       = 15.0f,
                float       sensitivity = 0.1f);

        glm::mat4   GetViewMatrix();
        glm::vec3   GetCameraPosition();
        glm::mat4   GetProjectionMatrix();
        void        Update(glm::vec3 position);
        void        HandleInput(GLFWwindow* window);
        
        
        glm::vec3   direction;
        glm::vec3   up;
        glm::vec3   right;
        glm::vec3   worldUp;
        float       speed;
        glm::vec3   position;

        double      lastX;
        double      lastY;
        bool        rotate;

        float       width;
        float       height;
        
    private:
        

        float       yaw;
        float       pitch;
        float       sensitivity;
        glm::mat4   projectionMatrix;
        unsigned int radius;
};