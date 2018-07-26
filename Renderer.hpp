#pragma once

#include <memory>
#include "Scene.hpp"
#include "Shader.hpp"

class Renderer
{
    public:

        Renderer(const char* vertexFile, const char* fragmentFile);

        void Draw(std::unique_ptr<Scene>& scene);

    private:

        Shader shader;
};