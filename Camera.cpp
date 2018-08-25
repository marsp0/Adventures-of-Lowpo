#include "Camera.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>

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
    std::cout << std::endl;
    float velocity = this->speed * deltaTime;
    if (direction == FORWARD)
        // Projection of direction/right onto the flat XZ plane plane
        this->position += (this->direction - (glm::dot(this->direction,this->worldUp) * this->worldUp)) * velocity;
    if (direction == BACKWARD)
        this->position -= (this->direction - (glm::dot(this->direction,this->worldUp) * this->worldUp)) * velocity;
    if (direction == LEFT)
        this->position -= (this->right - (glm::dot(this->right,this->worldUp) * this->worldUp)) * velocity;
    if (direction == RIGHT)
        this->position += (this->right - (glm::dot(this->right,this->worldUp) * this->worldUp)) * velocity;
}

void Camera::Update()
{
    glm::vec3 newDirection;
    newDirection.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    newDirection.y = sin(glm::radians(this->pitch));
    newDirection.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->direction = glm::normalize(newDirection);
    this->right = glm::normalize(glm::cross(this->direction,this->worldUp));
    this->up = glm::normalize(glm::cross(this->right,this->direction));
}