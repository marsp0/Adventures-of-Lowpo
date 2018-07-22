#include "Texture.hpp"
#include <iostream>

Texture::Texture(const std::string& fileName)
{
    // generate a texture and buffer the data.
    glGenTextures(1,&this->ID);
    glBindTexture(GL_TEXTURE_2D, this->ID);

    // make configurable
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR);

    int nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(),&this->width, &this->height,&nrChannels,0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,this->width,this->height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Texture was not loaded : " << fileName << std::endl;
    }
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1,&this->ID);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D,this->ID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D,0);
}
