#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>

#include "Terrain.hpp"

class ResourceManager
{
    public:
        ResourceManager();

        GLuint LoadMesh( const std::string& filePath, std::vector<std::shared_ptr<GameObject>>& gameObjects);
        
        std::shared_ptr<Terrain> LoadTerrain(const std::string& filePath);
};