#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "Shader.hpp"
#include "Camera.hpp"
#include "../Messaging/Message.hpp"

class Entity;
class RenderingSystem
{
    public:
        RenderingSystem();
        ~RenderingSystem();
        void AddShaders(std::vector<std::string> shaders, std::vector<std::string> shadowShaders);
        void Update(std::vector<std::shared_ptr<Entity>>& entities,
                    int playerID,
                    std::vector<Message>& messages,
                    std::vector<Message>& globalQueue);
        unsigned int CreateTexture(std::string filename);
        void HandleMessages(std::vector<Message> messages);
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

        Camera camera;
};