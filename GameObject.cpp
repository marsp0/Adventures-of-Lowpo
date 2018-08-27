#include "GameObject.hpp"

GameObject::GameObject( Transform                   transform, 
                        std::shared_ptr<Texture>    texture, 
                        std::shared_ptr<Mesh>       mesh,
                        PhysicsComponent physicsComponent) :
    transform(transform), physicsComponent(physicsComponent)
{
    this->mesh = mesh;
    if (texture != NULL)
    {
        this->texture = texture;
    }
}

void GameObject::HandleInput(GLFWwindow* window)
{
    std::cout << "called Gameobject HandleInput" << std::endl;
}

void GameObject::Update(float deltaTime)
{
    this->transform.SetPosition(this->physicsComponent.GetPosition());
}

void GameObject::Render()
{
    // this->texture->Bind();
    this->mesh->Bind();
    this->mesh->Draw();
    this->mesh->Unbind();
    // this->texture->Unbind();

}

void GameObject::SetVelocity(glm::vec3 velocity)
{
    std::cout << "Velocity is being set" << std::endl;
    this->physicsComponent.SetVelocity(velocity);
}

glm::vec3 GameObject::GetVelocity()
{
    return this->physicsComponent.GetVelocity();
}

glm::vec3 GameObject::GetPosition()
{
    return this->physicsComponent.GetPosition();
}