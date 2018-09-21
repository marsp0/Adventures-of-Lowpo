#include <GL/glew.h>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>

#include "Player.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "OBJ_Loader.hpp"
#include "ofbx.hpp"

ResourceManager::ResourceManager()
{
    
}

void ResourceManager::LoadMesh(const std::string& filePath, std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    objl::Loader loader;
    loader.LoadFile(filePath);
    for (int j = 0; j < loader.LoadedMeshes.size(); j++)
    {
        std::vector<float> data;
        glm::vec3 min, max;
        for (int i = 0; i < loader.LoadedMeshes[j].Vertices.size(); i++)
        {
            if (i == 0)
            {
                max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

            } else {
                if (max.x < loader.LoadedMeshes[j].Vertices[i].Position.X)
                    max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                if (max.y < loader.LoadedMeshes[j].Vertices[i].Position.Y)
                    max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                if (max.z < loader.LoadedMeshes[j].Vertices[i].Position.Z)
                    max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                if (min.x > loader.LoadedMeshes[j].Vertices[i].Position.X)
                    min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                if (min.y > loader.LoadedMeshes[j].Vertices[i].Position.Y)
                    min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                if (min.z > loader.LoadedMeshes[j].Vertices[i].Position.Z)
                    min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;
            }

            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.X);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Y);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Z);

            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.X);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Y);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Z);
        }

        unsigned int VAO,VBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float),data.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)3);

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VAO, VBO, loader.LoadedMeshes[j].Vertices.size()));
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(GameObject(Transform(), NULL, mesh, PhysicsComponent(min,max, glm::vec3(0.f,0.f,0.f))));
        gameObjects.push_back(gameObject);
    }
    
}

std::shared_ptr<Terrain> ResourceManager::LoadTerrain(const std::string& filePath, int gridSize, int cellSize)
{

    // TODO (Martin) : update terrain code to use planes in a quadtree

    // build the heightmap
    
    std::shared_ptr<std::vector<std::vector<float>>> map = std::make_shared<std::vector<std::vector<float>>>(gridSize);
    for (int i = 0; i < gridSize; i++)
    {
        (*map)[i] = std::vector<float>(gridSize);
    }
    objl::Loader loader;
    loader.LoadFile(filePath);

    std::vector<float>  data;
    
    for (int i = 0; i < loader.LoadedMeshes[0].Vertices.size(); i++)
    {
        int z = -((int)loader.LoadedMeshes[0].Vertices[i].Position.Z)/cellSize;
        int x = (int)loader.LoadedMeshes[0].Vertices[i].Position.X/cellSize;
        (*map)[z][x] = loader.LoadedMeshes[0].Vertices[i].Position.Y;
        (*map)[z][x] = loader.LoadedMeshes[0].Vertices[i].Position.Y;
    }   

    for (int i = 0; i < loader.LoadedMeshes[0].Vertices.size();i++)
    {
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Position.X);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Position.Y);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Position.Z);

        // Normals per face
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Normal.X);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Normal.Y);
        data.push_back(loader.LoadedMeshes[0].Vertices[i].Normal.Z);
    }
    unsigned int VAO, VBO;

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (float*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)(sizeof(float)*3));
    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(Terrain(VAO, VBO, data.size() / 6.0f, Transform(), map, cellSize,cellSize, position));
    return terrain;
}

void ResourceManager::LoadPlayer(const std::string& filePath, std::unique_ptr<Scene>& scene)
{
    objl::Loader loader;
    loader.LoadFile(filePath);
    for (int j = 0; j < loader.LoadedMeshes.size(); j++)
    {
        std::vector<float> data;
        glm::vec3 min, max;
        for (int i = 0; i < loader.LoadedMeshes[j].Vertices.size(); i++)
        {
            if (i == 0)
            {
                max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

            } else {
                if (max.x < loader.LoadedMeshes[j].Vertices[i].Position.X)
                    max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                if (max.y < loader.LoadedMeshes[j].Vertices[i].Position.Y)
                    max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                if (max.z < loader.LoadedMeshes[j].Vertices[i].Position.Z)
                    max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                if (min.x > loader.LoadedMeshes[j].Vertices[i].Position.X)
                    min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                if (min.y > loader.LoadedMeshes[j].Vertices[i].Position.Y)
                    min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                if (min.z > loader.LoadedMeshes[j].Vertices[i].Position.Z)
                    min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;
            }

            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.X);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Y);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Z);

            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.X);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Y);
            data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Z);
        }

        unsigned int VAO,VBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float),data.data(),GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,24,(void*)3);

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(VAO, VBO, loader.LoadedMeshes[j].Vertices.size()));
        std::shared_ptr<Player> player = std::make_shared<Player>(Player(Transform(), NULL, mesh, PhysicsComponent(min,max, glm::vec3(0.f,-10.f,0.f)),(float)800, (float)600));
        std::shared_ptr<Camera> camera = player->GetCamera();
        scene->AddCamera(camera);
        scene->gameObjects.push_back(player);
    }
    
}

std::vector<float> ResourceManager::GenerateNormals(std::vector<float>& vertices, int width, int length)
{
    std::vector<float> normals;
    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {
            if (z == 0 || z == length-1 || x == 0 || x == width-1)
            {
                // handle outer cases
                // outer non corner
                // outer corner

                normals.push_back(0.f);
                normals.push_back(1.f);
                normals.push_back(0.f);
            }
            else
            {
                // handle inner cases
                // get current index
                // get 6 vectors touching the current vertex
                // compute their normals and sum them together
                int currentIndex = (z * width + x) * 3;

                // FIRST
                glm::vec3 first;
                first.x = vertices[currentIndex - width * 3] - vertices[currentIndex];
                first.y = vertices[currentIndex - width * 3 + 1] - vertices[currentIndex+1];
                first.z = vertices[currentIndex - width * 3 + 2] - vertices[currentIndex+2];
                
                // SECOND
                glm::vec3 second;
                second.x = vertices[currentIndex + 3] - vertices[currentIndex];
                second.y = vertices[currentIndex + 4] - vertices[currentIndex+1];
                second.z = vertices[currentIndex + 5] - vertices[currentIndex+2];

                // FOURTH
                glm::vec3 fourth;
                fourth.x = vertices[currentIndex + width*3] - vertices[currentIndex];
                fourth.x = vertices[currentIndex + width*3 + 1] - vertices[currentIndex + 1];
                fourth.x = vertices[currentIndex + width*3 + 2] - vertices[currentIndex + 2];

                // FIFTH
                glm::vec3 fifth;
                fifth.x = vertices[currentIndex - 3] - vertices[currentIndex];
                fifth.y = vertices[currentIndex - 2] - vertices[currentIndex + 1];
                fifth.z = vertices[currentIndex - 1] - vertices[currentIndex + 2];

                glm::vec3 result(0.0f,0.0f,0.0f);
                result += glm::cross(second,first);
                result += glm::cross(first,fifth);
                result += glm::cross(fifth,fourth);
                result += glm::cross(fourth,second);
                result = glm::normalize(result);
                normals.push_back(result.x);
                normals.push_back(result.y);
                normals.push_back(result.z);
            }
        }
    }
    return normals;
}

void ResourceManager::LoadAnimatedObject(const std::string& filePath)
{
    std::ifstream       file;
    std::stringstream   fileString;

    // open the file and load the content in the buffer.
    file.open(filePath,std::ios_base::binary);
    fileString << file.rdbuf();

    // get the size
    file.seekg(0,std::ios_base::end);
    int size = file.tellg();
    file.seekg(0, std::ios_base::beg);

    char* cstring = new char[size];
    fileString.read(cstring,size);
    
    ofbx::IScene* scene = ofbx::load((ofbx::u8*)cstring,size);
    for (int i=0; i < scene->getAnimationStackCount() ; i++)
    {
        const ofbx::Object* stack = scene->getAnimationStack(i);
    }
}