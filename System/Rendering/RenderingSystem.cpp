#include <vector>
#include <GL/glew.h>

#include "RenderingSystem.hpp"
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
    this->shadowWidth = 2048;
    this->shadowHeight = 2048;

    // TODO : Make shadows appear
}

RenderingSystem::~RenderingSystem()
{

}

void RenderingSystem::Update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities)
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
            // draw entities
        }
    }
}