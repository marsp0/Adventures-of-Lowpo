#include "RenderingComponent.hpp"

RenderingComponent::RenderingComponent( unsigned int vertexArray,
                                        unsigned int vertexBuffer,
                                        unsigned int vertexCount,
                                        unsigned int textureID,
                                        ShaderType   shader) : \
                                        vertexArrayID(vertexArray),
                                        vertexBufferID(vertexBuffer),
                                        vertexCount(vertexCount),
                                        textureID(textureID),
                                        shader(shader),
                                        Component(ComponentType::Rendering)
{

}

unsigned int RenderingComponent::GetVertexArrayID()
{
    return this->vertexArrayID;
}

unsigned int RenderingComponent::GetVertexBufferID()
{
    return this->vertexBufferID;
}

unsigned int RenderingComponent::GetVertexCount()
{
    return this->vertexCount;
}

unsigned int RenderingComponent::GetTextureID()
{
    return this->GetTextureID;
}

ShaderType RenderingComponent::GetShaderType()
{
    return this->shader;
}