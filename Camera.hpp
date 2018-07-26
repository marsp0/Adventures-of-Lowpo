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
        
        Camera( float       width,
                float       height,
                glm::vec3   position    = glm::vec3(0.0f,0.0f,0.0f),
                glm::vec3   direction   = glm::vec3(0.0f,0.0f,3.0f),
                glm::vec3   worldUp     = glm::vec3(0.0f,1.0f,0.0f),
                GLfloat     yaw         = 0.0f,
                GLfloat     pitch       = 90.0f,
                GLfloat     sensitivity = 0.1f,
                GLfloat     speed       = .5f
                );

        glm::mat4   GetViewMatrix();
        glm::mat4   GetProjectionMatrix();
        void        Update();
        void        HandleMouseInput(float deltaTime,float xoffset, float yoffset);
        void        HandleKeyboardInput(float deltaTime,CameraMovement direction);

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
        glm::mat4   projectionMatrix;
        float       width;
        float       height;
};