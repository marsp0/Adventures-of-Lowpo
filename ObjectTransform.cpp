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
    glm::mat4 rotationMat = glm::mat4_cast(this->rotation);
    result *=  translation * rotationMat * scale;
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
    glm::mat4 rotationMat = glm::mat4_cast(this->rotation);
    result *=  translation * rotationMat * scale;
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

glm::vec3 Transform::DecomposeScale(glm::mat4 matrix)
{
    glm::vec3 scaleX = glm::vec3(matrix[0].x,matrix[0].y,matrix[0].z);
    glm::vec3 scaleY = glm::vec3(matrix[1].x,matrix[1].y,matrix[1].z);
    glm::vec3 scaleZ = glm::vec3(matrix[2].x,matrix[2].y,matrix[2].z);

    return glm::vec3(glm::length(scaleX), glm::length(scaleZ), glm::length(scaleZ));
}

glm::vec3 Transform::DecomposeTranslation(glm::mat4 matrix)
{
    glm::vec3 result = glm::vec3(matrix[3].x,matrix[3].y,matrix[3].z);
    return result;
}

glm::mat4 Transform::DecomposeRotation(glm::mat4 matrix, glm::vec3 scale)
{
    matrix[0].x /= scale.x;
    matrix[0].y /= scale.x;
    matrix[0].z /= scale.x;

    matrix[1].x /= scale.y;
    matrix[1].y /= scale.y;
    matrix[1].z /= scale.y;

    matrix[2].x /= scale.z;
    matrix[2].y /= scale.z;
    matrix[2].z /= scale.z;
    return matrix;
}