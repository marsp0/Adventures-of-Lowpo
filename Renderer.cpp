#include "Renderer.hpp"

Renderer::Renderer(const char* vertexFile, const char* fragmentFile) :
    shader(vertexFile, fragmentFile)
{
}

void Renderer::Draw(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain)
{
    glm::mat4 projection = scene->camera->GetProjectionMatrix();
    glm::mat4 view = scene->camera->GetViewMatrix();
    this->shader.Use();
    this->shader.SetMat4("projection", projection);
    this->shader.SetMat4("view", view);

    // light
    this->shader.SetVector3f("lightColor",1.f,1.f,1.f);
    this->shader.SetVector3f("lightPosition", 128.f,128.f,-128.f);
    this->shader.SetVector3f("viewPosition", scene->camera->position);

    // terrain render
    this->shader.SetMat4("model", terrain->transform.getWorldMatrix());
    this->shader.SetVector3f("ambientColor", .749f, .839f, .2f);
    terrain->Render();

    // game objects render
    for (int i = 0 ; i < scene->gameObjects.size(); i++)
    {   
        glm::mat4 model = scene->gameObjects[i]->transform.getWorldMatrix();   
        this->shader.SetMat4("model", model);
        this->shader.SetVector3f("ambientColor", scene->gameObjects[i]->material.ambient);
        scene->gameObjects[i]->Render();
    }
}
