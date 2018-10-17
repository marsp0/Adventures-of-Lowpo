#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
class Player;
class Camera
{
    public:
        
        Camera( float       width,
                float       height,
                glm::vec3   position ,
                glm::vec3   direction,
                glm::vec3   worldUp     = glm::vec3(0.0f,1.0f,0.0f),
                GLfloat     yaw         = 90.0f,
                GLfloat     pitch       = 15.0f,
                GLfloat     sensitivity = 0.1f,
                GLfloat     speed       = 2.5f
                );

        glm::mat4   GetViewMatrix();
        glm::mat4   GetProjectionMatrix();
        void        Update(Player* parent);
        void        HandleInput(GLFWwindow* window);
        glm::vec3   GetCameraPosition();
        
        
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