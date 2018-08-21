#pragma once

#include <memory>
#include "Scene.hpp"
#include "Shader.hpp"
#include "Terrain.hpp"

class Renderer
{
    public:

        Renderer(const char* vertexFile, const char* fragmentFile);

        void Draw(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain);

    private:

        Shader shader;
};