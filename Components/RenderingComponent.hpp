#pragma once

#include "Component.hpp"

enum ShaderType
{
    NormalShader,
    AnimatedShader
};

class RenderingComponent : public Component
{
    public:

        RenderingComponent( unsigned int vertexArray, 
                            unsigned int vertexBuffer, 
                            unsigned int vertexCount, 
                            unsigned int textureID,
                            ShaderType   shader);
        ~RenderingComponent();

        ShaderType   shader;
        unsigned int vertexArrayID;
        unsigned int vertexBufferID;
        unsigned int vertexCount;
        unsigned int textureID;

    private:
};