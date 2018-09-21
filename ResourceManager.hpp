#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>

#include "Terrain.hpp"
#include "Scene.hpp"

class ResourceManager
{
    public:
        ResourceManager();

        void                        LoadMesh( const std::string& filePath, std::vector<std::shared_ptr<GameObject>>& gameObjects);
        void                        LoadPlayer( const std::string& filePath, std::unique_ptr<Scene>& scene);
        std::shared_ptr<Terrain>    LoadTerrain(const std::string& filePath, int gridSize, int cellSize);
        std::vector<float>          GenerateNormals(std::vector<float>& vertices, int width, int length);
        void                        LoadAnimatedObject(const std::string& filePath);
};