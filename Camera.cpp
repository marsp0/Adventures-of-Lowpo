#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Camera.hpp"
#include "Player.hpp"

Camera::Camera( float       width,
                float       height,
                glm::vec3   position,
                glm::vec3   direction,
                glm::vec3   worldUp,
                GLfloat     yaw,
                GLfloat     pitch,
                GLfloat     sensitivity,
                GLfloat     speed
                ) : 
                position(position), direction(direction),
                worldUp(worldUp), yaw(yaw), pitch(pitch),
                sensitivity(sensitivity), speed(speed),
                width(width), height(height)
{
    this->radius = 10;

    this->rotate    = false;
    this->lastX     = width/2.0;
    this->lastY     = height/2.0;

    this->right     = glm::normalize(glm::cross(this->direction, this->worldUp));
    this->up        = glm::normalize(glm::cross(this->direction, this->right));
    this->projectionMatrix = glm::perspective(glm::radians(45.0f), this->width / this->height,.01f,200.f);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->position,this->position + this->direction,this->up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return this->projectionMatrix;
}

void Camera::Update(Player* parent)
{
    
    this->position = parent->GetPosition() + this->GetCameraPosition();
    this->direction = glm::normalize(parent->GetPosition() - this->position);
    this->right = glm::normalize(glm::cross(this->direction,this->worldUp));
    this->up = glm::normalize(glm::cross(this->right,this->direction));
}

void Camera::HandleInput(GLFWwindow* window)
{
    // MOUSE STUFF
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    this->rotate = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
    {
        // disable mouse to easily rotate
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        this->rotate = true;
        this->yaw += this->sensitivity * (x - this->lastX);
        this->pitch += this->sensitivity * (y - this->lastY);
    }
    if (this->pitch > 89.0f)
    {
        this->pitch = 89.0f;
    }
    if (this->pitch < -89.0f)
    {
        this->pitch = -89.f;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
    {
        // enable mouse on release
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    this->lastY = y;
    this->lastX = x;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        this->radius -= 5;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        this->radius += 5;
    }


}

glm::vec3 Camera::GetCameraPosition()
{
    /* 
r               - vector representing the change in the angles
    theta(pitch)- angle between r and xz plane
    p           - projection of r onto xz.   IMPORTANT -> p = r * cos(theta)
    phi(yaw)    - angle between p and x plane

    x = p * cos(yaw) = r * cos(pitch) * cos(yaw)
    y = r * sin(pitch)
    z = p * sin(yaw) =  r * cos(pitch) * sin(yaw)

    https://learnopengl.com/Getting-started/Camera
    */

    glm::vec3 result;
    result.x = this->radius * cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    result.y = this->radius * sin(glm::radians(this->pitch));
    result.z = this->radius * cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    return result;
}