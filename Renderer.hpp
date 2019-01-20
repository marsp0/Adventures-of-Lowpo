#pragma once

#include <memory>
#include <unordered_map>
#include "Scene.hpp"
#include "Shader.hpp"
#include "Terrain.hpp"

class Renderer
{
    public:

        Renderer(const char* vertexFile, const char* fragmentFile, 
                const char* vertexShadowFile, const char* fragmentShadowFile, 
                const char* vertexAnimationFile, const char* fragmentAnimationFile,
                const char* vertexAnimationShadowFile, const char* fragmentAnimationShadowFile,
                int width, int height);

        void Draw(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain);
        void DrawShadows(std::unique_ptr<Scene>& scene, std::shared_ptr<Terrain> terrain);

    private:

        std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
        std::shared_ptr<Shader> currentShader;
        unsigned int frameBuffer;
        unsigned int depthMapTexture;
        unsigned int shadowWidth;
        unsigned int shadowHeight;
        glm::mat4    lightSpaceMatrix;

        int width;
        int height;
};