#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>

#include "Terrain.hpp"
#include "Scene.hpp"
#include "Bone.hpp"
#include "tinyxml2.hpp"

namespace objl{
    class Vertex;
}

class ResourceManager
{
    public:
        ResourceManager();

        std::shared_ptr<Terrain>                LoadWorld(const std::string& filePath, std::unique_ptr<Scene>& scene);
        void                                    LoadPlayer(std::vector<objl::Vertex>& vertices, std::unique_ptr<Scene>& scene, std::shared_ptr<Texture> texture);
        std::shared_ptr<Terrain>                LoadTerrain(std::vector<objl::Vertex>& vertices, int gridSize, int cellSize);
        std::pair<unsigned int,unsigned int>    SetupBuffers(float* data, int size, bool animated);
        unsigned int                            LoadTexture(const std::string& filePath);
        void                                    LoadAnimatedObject(std::string filePath);
        std::shared_ptr<BoneTreeNode>           ParseNode(tinyxml2::XMLElement* node, std::vector<std::shared_ptr<Bone>>& bonesVector, glm::mat4 parentOffset);
        std::vector<std::string>                SplitString(const std::string& stringData);
        std::vector<int>                        SplitStringInt(const std::string& stringData);
        std::vector<float>                      SplitStringFloat(const std::string& stringData);
};