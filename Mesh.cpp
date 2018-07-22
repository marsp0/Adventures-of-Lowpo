#include "Mesh.hpp"
#include "OBJ_Loader.hpp"

Mesh::Mesh(const std::string& filePath)
{
    std::vector<float> data;
    objl::Loader loader;
    loader.LoadFile(filePath);
    
    glGenVertexArrays(1,&this->vertexArray);
    this->Bind();
    this->vertexCount = loader.LoadedMeshes[0].Vertices.size();

    glGenBuffers(1,&this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);

    // float max= 0.0f;
    // for (int i = 0; i < this->vertexCount; i++)
    // {
    //     if (loader.LoadedMeshes[0].Vertices[i].Position.X > max)
    //         max = loader.LoadedMeshes[0].Vertices[i].Position.X;
    //     if (loader.LoadedMeshes[0].Vertices[i].Position.Y > max)
    //         max = loader.LoadedMeshes[0].Vertices[i].Position.Y;
    //     if (loader.LoadedMeshes[0].Vertices[i].Position.Z > max)
    //         max = loader.LoadedMeshes[0].Vertices[i].Position.Z;
    // }
    // std::cout<<max<<std::endl;

    for (int i = 0; i < this->vertexCount; i++)
    {
        // positions
        // std::cout << loader.LoadedMeshes[0].Vertices[i].Position.X/max << std::endl;
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Position.X);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Position.Y);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Position.Z);
        // normals - not sure if needed as of now.
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Normal.X);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Normal.Y);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Normal.Z);
        // texture coords
        data.push_back(loader.LoadedMeshes[0].Vertices[i].TextureCoordinate.X);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].TextureCoordinate.Y);
    }

    // data = {
    //     -0.5f, -0.5f, 0.0f, 
    //     0.5f, -0.5f, 0.0f,  
    //     0.5f,  0.5f, 0.0f,  
    //     -0.5f, -0.5f, 0.0f, 
    //     -0.5f,  0.5f, 0.0f, 
    //     0.5f,  0.5f, 0.0f
    // };
    // this->vertexCount = 6.0f;

    // send data to the video memory
    glBufferData(GL_ARRAY_BUFFER,loader.LoadedMeshes[0].Vertices.size() * 8 * sizeof(float), &data[0], GL_STATIC_DRAW);
    glGetError();
    // shader attributes
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,32,(void*)0);
    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,32,(void*)(3 * sizeof(float)));
    // texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,32,(void*)(6 * sizeof(float)));

    this->Unbind();

}

void Mesh::Draw()
{
    this->Bind();
    glDrawArrays(GL_TRIANGLES,0,this->vertexCount);
    this->Unbind();
}

void Mesh::Bind()
{
    glBindVertexArray(this->vertexArray);
}

void Mesh::Unbind()
{
    glBindVertexArray(0);
}