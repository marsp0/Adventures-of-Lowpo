#include "GameObject.hpp"

GameObject::GameObject( Transform                   transform, 
                        std::shared_ptr<Texture>    texture, 
                        std::shared_ptr<Mesh>       mesh,
                        RigidBody                   rigidBody,
                        std::string shader,
                        std::string shadowShader) :
    transform(transform), rigidBody(rigidBody), shader(shader), shadowShader(shadowShader)
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

}

void GameObject::Render(std::shared_ptr<Shader> shader)
{
    this->texture->Bind();
    this->mesh->Bind();
    this->mesh->Draw();
    this->mesh->Unbind();
    this->texture->Unbind();

}