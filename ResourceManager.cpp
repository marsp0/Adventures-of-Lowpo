#include <GL/glew.h>
#include <memory>
#include <vector>

#include "GameObject.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "OBJ_Loader.hpp"

ResourceManager::ResourceManager()
{
    
}

GLuint ResourceManager::LoadMesh(const std::string& filePath, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    std::vector<float> data;
    objl::Loader loader;
    loader.LoadFile(filePath);
    std::cout << loader.LoadedMeshes.size() << std::endl;
    for (int j = 0 ; j < loader.LoadedMeshes.size(); j++)
    {
        
        GLuint vertexArrayID;
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        GLuint vertexBufferID;
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

        std::vector<float> data;
        glm::vec3 min;
        glm::vec3 max;
        for (int i = 0; i < loader.LoadedMeshes[j].Vertices.size(); i++)
        {
            if (i = 0)
            {
                max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;
            }

            if (min.x > loader.LoadedMeshes[j].Vertices[i].Position.X)
                min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
            if (min.y > loader.LoadedMeshes[j].Vertices[i].Position.Y)
                min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
            if (min.z > loader.LoadedMeshes[j].Vertices[i].Position.Z)
                min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

            if (max.x > loader.LoadedMeshes[j].Vertices[i].Position.X)
                max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
            if (max.y > loader.LoadedMeshes[j].Vertices[i].Position.Y)
                max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
            if (max.z > loader.LoadedMeshes[j].Vertices[i].Position.Z)
                max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.X);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Y);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Z);
            // normals - not sure if needed as jf now.
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.X);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Y);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Z);
        }
        glBufferData(GL_ARRAY_BUFFER,loader.LoadedMeshes[j].Vertices.size() * 6 * sizeof(float), &data[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,24,(void*)0);
        // normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,24,(void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(vertexArrayID, vertexBufferID, loader.LoadedMeshes[j].Vertices.size()));
        // std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(GameObject(Transform(), NULL, mesh, PhysicsComponent(min,max, glm::vec3(0.f,0.f,0.f))));
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(GameObject(Transform(), NULL, mesh));
        gameObject->transform.Scale(glm::vec3(20.f,22.f,22.f));
        gameObject->transform.SetPosition(glm::vec3(0.0f,0.5f,0.0f));
        gameObjects.push_back(gameObject);
        
    }
    // std::cout << loader.LoadedMeshes.size() << std::endl;
    
}

std::shared_ptr<Terrain> ResourceManager::LoadTerrain(const std::string& filePath)
{
    int nrChannels;
    unsigned int vertexCount;
    int width;
    int length;
    float w = 10.0f, l = 10.0f;
    unsigned char* data = stbi_load(filePath.c_str(),&width, &length,&nrChannels,0);
    std::vector<glm::vec3> vertices(width * length);
    std::vector<unsigned int> triangles;
    
    if (data == NULL)
    {
        std::cout << "Cannot load the heightmap!" << std::endl;
    }

    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {
            // normalizing the x and z of the vector
            glm::vec3 vertex = glm::vec3(x/(float)width,0,z/(float)length);

            // multiplying by the width and length
            vertex.x *= w;
            vertex.z *= l;
            // shifting the center to be in the center of the object.
            vertex.x -= width/2;
            vertex.z -= length/2;

            vertices[vertexCount] = vertex;

            // unsigned char* pixelOffset = data + (z + this->length * x) * nrChannels;
            // std::cout << "red " << static_cast<unsigned int>(pixelOffset[0]) << std::endl;
            
            if ((vertexCount+1 % width) != 0 && z+1 < length)
            {
                triangles.push_back(vertexCount);
                triangles.push_back(vertexCount + width);
                triangles.push_back(vertexCount + width + 1);

                triangles.push_back(vertexCount);
                triangles.push_back(vertexCount + width + 1);
                triangles.push_back(vertexCount + 1);
            }
            vertexCount++;
        }
    }
    stbi_image_free(data);

    // OPENGL Part
    GLuint IBO, VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(Terrain(VBO, IBO, triangles.size(), Transform()));
    terrain->transform.SetScale(glm::vec3(1.0f,1.0f,1.0f));
    terrain->transform.SetPosition(glm::vec3(0.0f,0.0f,0.0f));
    std::cout << triangles.size() << std::endl;
    return terrain;
}