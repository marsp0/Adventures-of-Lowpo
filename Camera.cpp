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
    this->right = glm::normalize(glm::cross(this->direction, this->worldUp));
    this->up = glm::normalize(glm::cross(this->direction, this->right));
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
    this->position = parent->GetPosition() + glm::vec3(0.f,10.f,12.f);
    
    this->direction = glm::normalize(parent->GetPosition() - this->position);
    this->right = glm::normalize(glm::cross(this->direction,this->worldUp));
    this->up = glm::normalize(glm::cross(this->right,this->direction));
}

