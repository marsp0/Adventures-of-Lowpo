#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
    public:
        
        Camera( glm::vec3   position    = glm::vec3(0.0f,0.0f,0.0f),
                glm::vec3   direction   = glm::vec3(0.0f,0.0f,3.0f),
                glm::vec3   worldUp     = glm::vec3(0.0f,1.0f,0.0f),
                GLfloat     yaw         = 0.0f,
                GLfloat     pitch       = 90.0f,
                GLfloat     sensitivity = 0.1f,
                GLfloat     speed       = 2.5f
                );

        void        Update();
        glm::mat4   GetViewMatrix();

        void HandleInput(float deltaTime, float xoffset, float yoffset, CameraMovement direction);
        void HandleMouseInput(float deltaTime,float xoffset, float yoffset);
        void HandleKeyboardInput(float deltaTime,CameraMovement direction);

    private:

        glm::vec3   position;
        glm::vec3   direction;
        glm::vec3   up;
        glm::vec3   right;
        glm::vec3   worldUp;
        GLfloat     yaw;
        GLfloat     pitch;
        GLfloat     sensitivity;
        GLfloat     speed;
};