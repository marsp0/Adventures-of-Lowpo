#pragma once

#include <GL/glew.h>
#include <string>
#include "stb_image.hpp"

class Texture
{
    public:
        Texture(const std::string& fileName);
        ~Texture();

        void Bind();
        void Unbind();

        // Data
        unsigned int  ID;
        
    private:
        int width;
        int height;
};