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
    float ambient = 0.5f;
    float diffuse = 0.4f;
    this->shader.SetVector3f("light.direction",-5.28f, -4.15f, 4.f);
    // this->shader.SetVector3f("light.direction",128.0f, 128.0f, -128.0f);
    this->shader.SetVector3f("light.ambient",ambient,ambient,ambient);
    this->shader.SetVector3f("light.diffuse",diffuse,diffuse,diffuse);

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
