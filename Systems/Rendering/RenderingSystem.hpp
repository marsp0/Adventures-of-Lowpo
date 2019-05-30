#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "Shader.hpp"

class Entity;
class RenderingSystem
{
    public:
        RenderingSystem(std::vector<std::string> shaders, std::vector<std::string> shadowShaders);
        ~RenderingSystem();

        void Update(std::vector<std::shared_ptr<Entity>>& entities);
        unsigned int CreateTexture(std::string filename);

        static std::pair<unsigned int, unsigned int> BufferData(float* data, int size, bool animated);

    private:

        std::uint32_t primaryBitset;

        // Shaders
        std::vector<Shader> shaders;
        std::vector<Shader> shadowShaders;

        // shadows
        unsigned int frameBuffer;
        unsigned int depthMapTexture;
        unsigned int shadowWidth;
        unsigned int shadowHeight;
        glm::mat4    lightSpaceTransform;

        int width;
        int height;

        // light
        float ambient;
        float diffuse;
        glm::vec3 lightDirection;

        std::unordered_map<std::string, unsigned int> loadedTextures;
};