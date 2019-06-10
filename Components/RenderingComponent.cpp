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

RenderingComponent::~RenderingComponent()
{
        
}