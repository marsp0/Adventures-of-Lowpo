#include "GameObject.hpp"

GameObject::GameObject( Transform                   transform, 
                        std::shared_ptr<Texture>    texture, 
                        std::shared_ptr<Mesh>       mesh,
                        PhysicsComponent physicsComponent,
                        std::string shader,
                        std::string shadowShader) :
    transform(transform), physicsComponent(physicsComponent), shader(shader), shadowShader(shadowShader)
{
    this->mesh = mesh;
    
    if (texture != NULL)
    {
        this->texture = texture;
    }
}

void GameObject::HandleInput(GLFWwindow* window)
{
}

void GameObject::Update(float deltaTime)
{
    this->transform.SetPosition(this->physicsComponent.GetPosition());
}

void GameObject::Render(std::shared_ptr<Shader> shader)
{
    this->texture->Bind();
    this->mesh->Bind();
    this->mesh->Draw();
    this->mesh->Unbind();
    this->texture->Unbind();

}

void GameObject::SetVelocity(glm::vec3 velocity)
{
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