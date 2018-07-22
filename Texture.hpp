#pragma once

#include <GL/glew.h>
#include <string>
#include "stb_image.hpp"

class Texture
{
    public:
        Texture(const std::string& fileName);
        ~Texture();

        void Bind() const;
        void Unbind() const;
    private:
        GLuint ID;
        GLsizei width;
        GLsizei height;
};