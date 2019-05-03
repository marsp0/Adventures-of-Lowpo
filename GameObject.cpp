#include "GameObject.hpp"

GameObject::GameObject( std::shared_ptr<Texture>    texture, 
                        std::shared_ptr<Mesh>       mesh,
                        std::shared_ptr<RigidBody>  rigidBody,
                        std::string shader,
                        std::string shadowShader) :
                        mesh(mesh),
                        texture(texture),
                        rigidBody(rigidBody), 
                        shader(shader), 
                        shadowShader(shadowShader)
{

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

glm::mat4 GameObject::GetWorldMatrix()
{
    return this->rigidBody->GetTransform();
}

glm::vec3 GameObject::GetPosition()
{
    return this->rigidBody->GetPosition();
}