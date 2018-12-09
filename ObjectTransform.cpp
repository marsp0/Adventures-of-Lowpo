#include "ObjectTransform.hpp"

Transform::Transform()
{
    this->scale = glm::vec3(1.0f,1.0f,1.0f);
    this->position = glm::vec3(0.0f,0.0f,0.0f);
    this->rotation = glm::quat();
}

// Same code as GetWorldMatrix
glm::mat4 Transform::GetMatrix()
{
    glm::mat4 scale = glm::mat4(
        this->scale.x, 0, 0, 0,
        0, this->scale.y, 0, 0,
        0, 0, this->scale.z, 0,
        0, 0, 0, 1
    );
    glm::mat4 translation = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        this->position.x, this->position.y, this->position.z, 1
    );
    glm::mat4 result(1.0f); 
    result *=  glm::mat4_cast(this->rotation) * scale * translation;
    return result;
}

glm::mat4 Transform::GetWorldMatrix()
{
    glm::mat4 scale = glm::mat4(
        this->scale.x, 0, 0, 0,
        0, this->scale.y, 0, 0,
        0, 0, this->scale.z, 0,
        0, 0, 0, 1
    );
    glm::mat4 translation = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        this->position.x, this->position.y, this->position.z, 1
    );
    glm::mat4 result(1.0f); 
    result *=  glm::mat4_cast(this->rotation) * scale * translation;
    return result;
}

void Transform::SetPosition(glm::vec3 position)
{
    this->position = position;
}

void Transform::SetRotation(glm::quat rotation)
{
    this->rotation = rotation;
}

glm::quat Transform::GetRotation()
{
    return this->rotation;
}

void Transform::Rotate(glm::quat rotation)
{
    this->rotation = glm::normalize(this->rotation * rotation);
}

void Transform::SetScale(glm::vec3 scale)
{
    this->scale = scale;
}

glm::vec3 Transform::GetScale()
{
    return this->scale;
}

void Transform::Scale(glm::vec3 scale)
{
    this->scale += scale;
}


