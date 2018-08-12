#pragma once

#include <GL/glew.h>
#include <string>

class ResourceManager
{
    public:
        ResourceManager();

        GLuint LoadMesh( const std::string& filePath, std::vector<std::shared_ptr<GameObject>>& gameObjects);
};