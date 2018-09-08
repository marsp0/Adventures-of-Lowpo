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
    this->shader.SetVector3f("lightPosition", 0.f,10.f,0.f);
    this->shader.SetVector3f("viewPosition", scene->camera->position);

    // terrain render
    this->shader.SetMat4("model", terrain->transform.getWorldMatrix());
    terrain->Render();

    // game objects render
    for (int i = 0 ; i < scene->gameObjects.size(); i++)
    {   
        glm::mat4 model = scene->gameObjects[i]->transform.getWorldMatrix();   
        this->shader.SetMat4("model", model);
        scene->gameObjects[i]->Render();
    }
}
