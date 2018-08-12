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
        for (int i = 0; i < loader.LoadedMeshes[j].Vertices.size(); i++)
        {
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
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(GameObject(Transform(), NULL, mesh));
        gameObject->transform.Scale(glm::vec3(5.f,5.f,5.f));
        gameObject->transform.SetPosition(glm::vec3(0.0f,0.5f,0.0f));
        gameObjects.push_back(gameObject);
        
    }
    std::cout << loader.LoadedMeshes.size() << std::endl;
    
}