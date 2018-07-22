#include "Camera.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>

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

void Camera::HandleInput(float deltaTime, float xoffset, float yoffset, CameraMovement direction)
{
    this->HandleMouseInput(deltaTime, xoffset, yoffset);
    this->HandleKeyboardInput(deltaTime, direction);
}

void Camera::HandleMouseInput(float deltaTime, float xoffset, float yoffset)
{
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (this->pitch > 89.f)
        this->pitch = 89.0f;
    if (this->pitch < -89.f)
        this->pitch = -89.0f;

    this->Update();
}

void Camera::HandleKeyboardInput(float deltaTime, CameraMovement direction)
{
    // std::cout << this->position.x << std::endl;
    float velocity = this->speed * deltaTime;
    if (direction == FORWARD)
        this->position += this->direction * velocity;
    if (direction == BACKWARD)
        this->position -= this->direction * velocity;
    if (direction == LEFT)
        this->position -= this->right * velocity;
    if (direction == RIGHT)
        this->position += this->right * velocity;
}

void Camera::Update()
{
    // std::cout << this->direction.x << std::endl;
    glm::vec3 newDirection;
    newDirection.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    newDirection.y = sin(glm::radians(this->pitch));
    newDirection.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->direction = glm::normalize(newDirection);
    this->right = glm::normalize(glm::cross(this->direction,this->worldUp));
    this->up = glm::normalize(glm::cross(this->right,this->direction));
}