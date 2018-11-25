#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.hpp"

Renderer::Renderer(const char* vertexFile, const char* fragmentFile, 
                    const char* vertexShadowFile, const char* fragmentShadowFile,
                    int width, int height) :
    shader(vertexFile, fragmentFile),
    shadowShader(vertexShadowFile,fragmentShadowFile),
    width(width), height(height)
{
}

void Renderer::Draw(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain)
{
    glViewport(0, 0, this->width, this->height);
    glClearColor(0.529f, 0.807f, 0.980f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection = scene->camera->GetProjectionMatrix();
    glm::mat4 view = scene->camera->GetViewMatrix();
    this->shader.Use();
    this->shader.SetMat4("projection", projection);
    this->shader.SetMat4("view", view);
    // this->shader.SetMat4("lightSpaceMatrix", this->lightSpaceMatrix);

    // textures
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, this->depthMapTexture);

    // light
    float ambient = 0.5f;
    float diffuse = 0.4f;
    this->shader.SetVector3f("light.direction",-5.28f, -4.15f, 4.f);
    this->shader.SetVector3f("light.ambient",ambient,ambient,ambient);
    this->shader.SetVector3f("light.diffuse",diffuse,diffuse,diffuse);

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