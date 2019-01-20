#pragma once

#include <memory>
#include "Scene.hpp"
#include "Shader.hpp"
#include "Terrain.hpp"

class Renderer
{
    public:

        Renderer(const char* vertexFile, const char* fragmentFile, const char* vertexShadowFile, const char* fragmentShadowFile, const char* vertexAnimationFile, const char* fragmentAnimationFile, int width, int height);

        void Draw(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain);
        void DrawShadows(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain);

    private:

        Shader shader;
        Shader shadowShader;
        Shader animationShader;

        unsigned int frameBuffer;
        unsigned int depthMapTexture;
        unsigned int shadowWidth;
        unsigned int shadowHeight;
        glm::mat4    lightSpaceMatrix;

        int width;
        int height;
};