#include "TransformComponent.hpp"

TransformComponent::TransformComponent(glm::vec3 position, 
                                       glm::quat orientation) : \
position(position),
orientation(orientation),
Component(ComponentType::Transform)
{
    
}

TransformComponent::~TransformComponent()
{
    
}

glm::mat4 TransformComponent::GetWorldTransform()
{
    glm::mat4 translation = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        this->position.x, this->position.y, this->position.z, 1
        );
    glm::mat4 result(1.0f);
    glm::mat4 rotationMat = glm::mat4_cast(this->orientation);
    result *=  translation * rotationMat;
    return result;
}