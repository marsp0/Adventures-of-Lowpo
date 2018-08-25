#include "ObjectTransform.hpp"
#include "Terrain.hpp"
#include "GameObject.hpp"

Transform::Transform(GameObject* parent)
{
    this->parentGameObject = parent;
    this->scale = glm::vec3(1.0f,1.0f,1.0f);
}

Transform::Transform(Terrain* parent)
{
    this->parentTerrain = parent;
    this->scale = glm::vec3(1.0f,1.0f,1.0f);
}

// Transform::Transform(const Transform& transform)
// {
//     this->parent = parent;
//     this->scale = transform.scale;
//     this->rotation = transform.rotation;
// }

glm::mat4 Transform::getWorldMatrix()
{
    glm::mat4 scale = glm::mat4(
        this->scale.x, 0, 0, 0,
        0, this->scale.y, 0, 0,
        0, 0, this->scale.z, 0,
        0, 0, 0, 1
    );
    glm::vec3 position = this->GetPosition();
    std::cout << position.x << std::endl;
    std::cout << position.y << std::endl;
    std::cout << position.z << std::endl;
    std::cout << std::endl;
    glm::mat4 translation = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        position.x, position.y, position.z, 1
    );
    glm::mat4 result = glm::mat4_cast(this->rotation) * scale * translation;
    return result;
}

glm::vec3 Transform::GetPosition()
{
    // TODO : Fix this mess - is it ok to use weak ptrs ? is the way i am doing this correct?
    // IMPORTANT
    if (this->parentGameObject != nullptr)
        return this->parentGameObject->GetPosition();
    if (this->parentTerrain != nullptr)
        return this->parentTerrain->GetPosition();
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


