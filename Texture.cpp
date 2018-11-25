#include "Texture.hpp"
#include <iostream>

Texture::Texture(const std::string& fileName) : ID()
{
    // generate a texture and buffer the data.
    glGenTextures(1,&this->ID);
    std::cout << this->ID << std::endl;
    glBindTexture(GL_TEXTURE_2D, this->ID);
    std::cout << "is the ID" << this->ID << std::endl;
    // make configurable
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fileName.c_str(),&this->width, &this->height,&nrChannels,0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,this->width,this->height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);   
    }
    else
    {
        std::cout << "Texture was not loaded : " << fileName << std::endl;
    }
    stbi_image_free(data);
    this->Unbind();
}

Texture::~Texture()
{
    // glDeleteTextures(1,&this->ID);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D,0);
}
