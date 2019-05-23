#include <vector>
#include <GL/glew.h>

#include "RenderingSystem.hpp"
#include "RenderingComponent.hpp"
#include "TransformComponent.hpp"
#include "../Entity.hpp"

RenderingSystem::RenderingSystem(std::vector<std::string> shaders, std::vector<std::string> shadowShaders)
{
    this->requiredBitset =  ComponentType::Rendering | \
                            ComponentType::Transform;
    assert(shaders.size() == shadowShaders.size());
    for (int i = 0; i < shaders.size(); i += 2)
    {
        this->shaders.push_back(Shader(shaders[i], shaders[i+1]));
        this->shadowShaders.push_back(Shader(shadowShaders[i], shadowShaders[i+1]));
    }
    this->width = 800;
    this->height = 600;
    this->ambient = 0.3f;
    this->diffuse = 0.5f;
    this->lightDirection = glm::vec3(-48.f,-128.f,0.f);
    // TODO : Make shadows appear
}

RenderingSystem::~RenderingSystem()
{

}

void RenderingSystem::Update(std::vector<std::shared_ptr<Entity>>& entities)
{
    // initial prep
    glViewport(0, 0, this->width, this->height);
    glClearColor(0.529f, 0.807f, 0.980f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO : Camera
    // set projection and view
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i]->IsEligibleForSystem(this->requiredBitset))
        {
            RenderingComponent renderingComponent = entities[i]->GetComponent<RenderingComponent>(ComponentType::Rendering);
            TransformComponent transformComponent = entities[i]->GetComponent<TransformComponent>(ComponentType::Transform);
            ShaderType shaderType = renderingComponent.GetShaderType();
            this->shaders[shaderType].Use();
            this->shaders[shaderType].SetVector3f("light.direction", this->lightDirection);
            this->shaders[shaderType].SetVector3f("light.ambient", ambient, ambient, ambient);
            this->shaders[shaderType].SetVector3f("light.diffuse", diffuse, diffuse, diffuse);
            this->shaders[shaderType].SetMat4("model", transformComponent.GetWorldTransform());

            // texture bind
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderingComponent.GetTextureID());

            // vao bind
            glBindVertexArray(renderingComponent.GetVertexArrayID());

            // draw
            glDrawArrays(GL_TRIANGLES, 0, renderingComponent.GetVertexCount());

            // unbind vao
            glBindVertexArray(0);

            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);

        }
    }
}