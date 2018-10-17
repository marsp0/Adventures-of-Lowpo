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
    glGenFramebuffers(1,&this->frameBuffer);
    
    glGenTextures(1, &this->depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, this->depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 
    float nearPlane = .01f;
    float farPlane = 200.f;
    glm::mat4 lightProjection = glm::ortho(-10.0f,10.f,-10.f, 10.f, nearPlane, farPlane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(128.f,128.f,-128.f),
                                      glm::vec3(1.f,1.f,-1.f),
                                      glm::vec3(0.f,1.0f,0.f));
    this->lightSpaceMatrix = lightProjection * lightView;
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
    this->shader.SetMat4("lightSpaceMatrix", this->lightSpaceMatrix);

    // textures
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->depthMapTexture);

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

void Renderer::DrawShadows(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain)
{
    glBindFramebuffer(GL_FRAMEBUFFER,this->frameBuffer);
    glViewport(0,0,1024,1024);
    glClear(GL_DEPTH_BUFFER_BIT);
    this->shadowShader.Use();
    
    this->shadowShader.SetMat4("lightSpaceMatrix", this->lightSpaceMatrix);
    
    // terrain render
    this->shadowShader.SetMat4("model", terrain->transform.getWorldMatrix());
    terrain->Render();

    // game objects render
    for (int i = 0 ; i < scene->gameObjects.size(); i++)
    {   
        glm::mat4 model = scene->gameObjects[i]->transform.getWorldMatrix();   
        this->shadowShader.SetMat4("model", model);
        scene->gameObjects[i]->Render();
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}