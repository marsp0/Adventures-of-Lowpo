#include <GL/glew.h>
#include <memory>
#include <vector>

#include "Player.hpp"
#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "OBJ_Loader.hpp"

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

std::shared_ptr<Terrain> ResourceManager::LoadTerrain(const std::string& filePath)
{
    // RGBA / RGB etc
    int nrChannels;
    int vertexCount(0);
    int width;
    int length;
    float worldWidth = 20.0f, worldLength = 20.0f, worldHeight = 2.0f;
    unsigned char* data = stbi_load(filePath.c_str(),&width, &length,&nrChannels,0);
    std::vector<float> vertices;
    std::vector<unsigned int> triangles;
    std::shared_ptr<std::vector<std::vector<float>>> heightmap = std::make_shared<std::vector<std::vector<float>>>(std::vector<std::vector<float>>());
    if (data == NULL)
    {
        std::cout << "Cannot load the heightmap!" << std::endl;
    }
    for (int z = 0; z < length; z++)
    {
        heightmap->push_back(std::vector<float>());
        for (int x = 0; x < width; x++)
        {
            // normalizing the x and z of the vector
            glm::vec3 vertex = glm::vec3(x/(float)width,0,z/(float)length);

            // multiplying by the width and length
            unsigned char* pixel = data + vertexCount * nrChannels;
            unsigned int temporary_y = static_cast<unsigned int>(pixel[0]);
            vertex.y = (float)temporary_y/255.0f;

            vertex.x *= worldWidth;
            vertex.y *= worldHeight;
            vertex.z *= worldLength;
            // shifting the center to be in the center of the object.
            vertex.x -= worldWidth/2;
            vertex.z -= worldLength/2;
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);
            if ((vertexCount+1 % width) != 0 && z < length-1)
            {
                triangles.push_back(vertexCount);
                triangles.push_back(vertexCount + width);
                triangles.push_back(vertexCount + width + 1);

                triangles.push_back(vertexCount);
                triangles.push_back(vertexCount + width + 1);
                triangles.push_back(vertexCount + 1);
            }

            (*heightmap)[z].push_back(vertex.y);

            vertexCount++;
        }
    }
    stbi_image_free(data);
    std::vector<float> normals;
    // TODO : Refactor the code ; A LOT OF REPETITION
    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {

            if (z < length - 1 || x < width - 1)
            {
                // std::cout << "z is " << z << " x is " << x << std::endl;
                int index = ((z)*width + x )* 3;
                glm::vec3 first = glm::vec3(vertices[index + 3] - vertices[index], vertices[index+4] - vertices[index+1], vertices[index+5] - vertices[index + 2]);
                glm::vec3 second = glm::vec3(vertices[index + width*3 + 3] - vertices[index], vertices[index+ width * 3 + 4] - vertices[index+1], vertices[index+width*3 + 5] - vertices[index + 2]);
                glm::vec3 normal = glm::normalize(glm::cross(first,second));
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            }
            else
            {
                if (z == length - 1 && x == width - 1)
                {
                    // std::cout << "z is " << z << " x is " << x << std::endl;
                    int index = ((z )*width + x )* 3;
                    glm::vec3 first = glm::vec3(vertices[index - 3] - vertices[index], vertices[index - 2] - vertices[index+1], vertices[index - 1] - vertices[index + 2]);
                    glm::vec3 second = glm::vec3(vertices[index - width * 3] - vertices[index], vertices[index - width * 3 + 1] - vertices[index+1], vertices[index - width * 3 + 2] - vertices[index + 2]);
                    glm::vec3 normal = glm::normalize(glm::cross(second,first));
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);
                }
                if (x == width - 1 && z != length - 1)
                {
                    // std::cout << "z is " << z << " x is " << x << std::endl;
                    int index = ((z )*width + x )* 3;
                    glm::vec3 first = glm::vec3(vertices[index - 3] - vertices[index], vertices[index - 2] - vertices[index+1], vertices[index - 1] - vertices[index + 2]);
                    glm::vec3 second = glm::vec3(vertices[index + width * 3] - vertices[index], vertices[index + width * 3 + 1] - vertices[index+1], vertices[index + width * 3 + 2] - vertices[index + 2]);
                    glm::vec3 normal = glm::normalize(glm::cross(second,first));
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);
                }
                if (z == length-1 && x != width - 1)
                {
                    // std::cout << "z is " << z << " x is " << x << std::endl;
                    int index = ((z )*width + x )* 3;
                    glm::vec3 first = glm::vec3(vertices[index - width * 3 + 3] - vertices[index], vertices[index - width * 3+4] - vertices[index+1], vertices[index - width * 3+5] - vertices[index + 2]);
                    glm::vec3 second = glm::vec3(vertices[index + 3] - vertices[index], vertices[index + 4] - vertices[index+1], vertices[index + 5] - vertices[index + 2]);
                    glm::vec3 normal = glm::normalize(glm::cross(second,first));
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);
                }
            }
        }
    }

    std::cout << normals.size() << std::endl;
    std::cout << vertices.size() << std::endl;

    // OPENGL Part
    GLuint IBO, VBO, VAO, normalVBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,(void*)0);

    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER,normalVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, (void*)3);

    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(Terrain(VAO, VBO, IBO, triangles.size(), Transform(), heightmap, worldWidth, worldHeight, worldLength, position));
    
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