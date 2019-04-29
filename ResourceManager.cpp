#include <GL/glew.h>

#include "Player.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Loader.hpp"
#include "AABB.hpp"
#include "Triangle.hpp"

ResourceManager::ResourceManager()
{
    
}

void ResourceManager::InitWorld()
{
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::string filename = "./resources/test.dae";
    // load file
    tinyxml2::XMLElement* collada = Loader::LoadFile(filename);
    if (collada == nullptr)
        return;
    
    // parse visual scenes for the world transforms.
    std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>> instanceGeometries;
    tinyxml2::XMLElement* libraryVisualScenes = collada->FirstChildElement("library_visual_scenes");
    instanceGeometries = Loader::ParseVisualScenesStatic(libraryVisualScenes);

    // parse geometries
    // one object - > many colliders
    std::unordered_map<std::string, std::vector<std::shared_ptr<Collider>> > objectToColliders;
    std::unordered_map<std::string, std::shared_ptr<Geometry>> geometry;
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    geometry = Loader::ParseGeometry(libraryGeometries);
    for (std::unordered_map<std::string, std::shared_ptr<Geometry>>::iterator it = geometry.begin(); it != geometry.end(); it++)
    {
        // Three scenarios
        // is hitbox
        // is not hitbox
        // is terrain
        bool isHitbox = it->first.find("_hitbox") != it->first.npos;
        bool isTerrain = it->first.find("Terrain") != it->first.npos;
        if (isHitbox)
        {
            int hitboxIndex = it->first.find("_hitbox");
            std::string objectName = it->first.substr(0, hitboxIndex);
            // take thhe vertices and make a collider.
            glm::vec3 center = glm::vec3(0.f,0.f,0.f);
            for (int i = 0; i < it->second->vertices.size(); i += 3)
            {
                float x = it->second->vertices[i];
                float y = it->second->vertices[i + 1];
                float z = it->second->vertices[i + 2];
                glm::vec3 point = glm::vec3(x,y,z);
                center += point;
            }
            int numOfPoints = it->second->vertices.size() / 3;
            center /= numOfPoints;
            float x = it->second->vertices[0];
            float y = it->second->vertices[1];
            float z = it->second->vertices[2];
            glm::vec3 axisRadii = glm::vec3(fabs(center.x - x), fabs(center.y - y), fabs(center.z - z));
            std::shared_ptr<AABB> collider = std::make_shared<AABB>(AABB(center, it->first, axisRadii, ColliderType::BOX, DynamicType::Static, nullptr));
            objectToColliders[objectName].push_back(collider);
        }
        else if (isTerrain)
        {

        }
        else
        {
            std::shared_ptr<Geometry> current = it->second;
            std::vector<float> bufferData;
            for (int i = 0; current->indices.size(); i += current->stride * 3)
            {
                std::vector<int> vertexIndices{current->indices[i],
                                                current->indices[i + current->stride],
                                                current->indices[i + 2 * current->stride]};
                std::vector<int> textureIndices{current->indices[i + 2],
                                                current->indices[i + 2 + current->stride],
                                                current->indices[i + 2 + 2 * current->stride]};
                std::vector<glm::vec3> tempVertices;
                std::vector<glm::vec2> tempTextureCoords;
                for (int j = 0; j < vertexIndices.size(); j++)
                {
                    int vertexIndex = vertexIndices[j] * 3;
                    int textureIndex = textureIndices[j] * 2;
                    float x = current->vertices[vertexIndex];
                    float y = current->vertices[vertexIndex + 1];
                    float z = current->vertices[vertexIndex + 2];

                    float s = current->texCoords[textureIndex];
                    float t = current->texCoords[textureIndex + 1];

                    tempVertices.push_back(glm::vec3(x,y,z));
                    tempTextureCoords.push_back(glm::vec2(s,t));
                }
                glm::vec3 normal = glm::cross(tempVertices[0] - tempVertices[1], tempVertices[2] - tempVertices[1]);

                for (int j = 0; j < tempVertices.size(); j++)
                {
                    bufferData.push_back(tempVertices[j].x);
                    bufferData.push_back(tempVertices[j].y);
                    bufferData.push_back(tempVertices[j].z);

                    bufferData.push_back(normal.x);
                    bufferData.push_back(normal.y);
                    bufferData.push_back(normal.z);

                    bufferData.push_back(tempTextureCoords[j].s);
                    bufferData.push_back(tempTextureCoords[j].t);
                }
            }
        }
    }
}

std::pair<unsigned int,unsigned int> ResourceManager::SetupBuffers(float* data, int size, bool animated)
{
    int floatCount = 8;
    if (animated)
    {
        floatCount = 16;
    }
    unsigned int VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatCount * sizeof(float),(void*)(sizeof(float)*3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*6));

    if (animated)
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*8));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*12));
    }
    return std::make_pair(VAO,VBO);
}