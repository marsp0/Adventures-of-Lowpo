#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>

#include "Terrain.hpp"
#include "Scene.hpp"

namespace objl{
    class Vertex;
}

class ResourceManager
{
    public:
        ResourceManager();

        std::shared_ptr<Terrain>                LoadWorld( const std::string& filePath, std::unique_ptr<Scene>& scene);
        void                                    LoadPlayer( std::vector<objl::Vertex>& vertices, std::unique_ptr<Scene>& scene, std::shared_ptr<Texture> texture);
        std::shared_ptr<Terrain>                LoadTerrain(std::vector<objl::Vertex>& vertices, int gridSize, int cellSize);
        std::vector<float>                      GenerateNormals(std::vector<float>& vertices, int width, int length);
        std::pair<unsigned int,unsigned int>    SetupBuffers(float* data, int size);
        void                                    LoadAnimatedObject(const std::string& filePath);
        unsigned int                            LoadTexture(const std::string& filePath);
};