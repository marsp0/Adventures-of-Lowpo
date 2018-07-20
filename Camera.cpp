#include "Camera.hpp"
#include <glm/gtx/transform.hpp>

Camera::Camera( glm::vec3   position,
                glm::vec3   direction,
                glm::vec3   worldUp,
                GLfloat     yaw,
                GLfloat     pitch,
                GLfloat     sensitivity,
                GLfloat     speed
                ) : 
                position(position), direction(direction),
                worldUp(worldUp), yaw(yaw), pitch(pitch),
                sensitivity(sensitivity), speed(speed)
{
    this->right = glm::normalize(glm::cross(this->direction, this->worldUp));
    this->up = glm::normalize(glm::cross(this->direction, this->right));
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->position,this->position + this->direction,this->up);
}

void Camera::Update()
{
    glm::vec3 newDirection;
    newDirection.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    newDirection.y = sin(glm::radians(this->pitch));
    newDirection.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->direction = glm::normalize(newDirection);
    this->right = glm::normalize(glm::cross(this->direction,this->worldUp));
    this->up = glm::normalize(glm::cross(this->direction,this->right));
}