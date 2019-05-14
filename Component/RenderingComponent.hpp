#pragma once

#include "Component.hpp"

enum ShaderType
{
    Normal,
    Animated
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

        unsigned int GetVertexArrayID();
        unsigned int GetVertexBufferID();
        unsigned int GetVertexCount();
        unsigned int GetTextureID();
        ShaderType   GetShaderType();

    private:

        ShaderType   shader;
        unsigned int vertexArrayID;
        unsigned int vertexBufferID;
        unsigned int vertexCount;
        unsigned int textureID;
};