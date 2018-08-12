#include "GameObject.hpp"
#include "GLFW/glfw3.h"

GameObject::GameObject(Transform transform, std::shared_ptr<Texture> texture, std::shared_ptr<Mesh> mesh) :
    transform(transform)
{
    this->mesh = mesh;
    if (texture != NULL)
    {
        this->texture = texture;
    }
    
}

void GameObject::HandleInput()
{

}

void GameObject::Update(float deltaTime)
{

}

void GameObject::Render()
{
    // this->texture->Bind();
    this->mesh->Bind();
    this->mesh->Draw();
    this->mesh->Unbind();
    // this->texture->Unbind();

}