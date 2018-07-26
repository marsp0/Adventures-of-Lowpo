#pragma once

#include <memory>
#include "Scene.hpp"

class Renderer
{
    public:

        Renderer();

        void Draw(std::unique_ptr<Scene> scene);

    private:
};