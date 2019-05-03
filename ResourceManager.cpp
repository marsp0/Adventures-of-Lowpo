#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "Player.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Loader.hpp"
#include "physics/AABB.hpp"
#include "physics/Triangle.hpp"

ResourceManager::ResourceManager()
{
    
}

void ResourceManager::InitWorld()
{
    // global texture
    std::string filename = "./resources/texture.jpg";
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture(filename));

    std::vector<std::shared_ptr<GameObject>> gameObjects;
    filename = "./resources/test.dae";
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
        bool createGameObject = true;
        bool isHitbox = it->first.find("_hitbox") != it->first.npos;
        bool isTerrain = it->first == "Terrain";
        std::vector<float> bufferData;
        glm::mat4 worldTransform;
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
            float x = fabs(center.x - it->second->vertices[0]);
            float y = fabs(center.y - it->second->vertices[1]);
            float z = fabs(center.z - it->second->vertices[2]);
            glm::vec3 axisRadii = glm::vec3( x, y, z);
            center = instanceGeometries[it->first]->matrix * glm::vec4(center, 1.0f);
            std::shared_ptr<AABB> collider = std::make_shared<AABB>(AABB(center, it->first, axisRadii, ColliderType::BOX, DynamicType::Static, nullptr));
            objectToColliders[objectName].push_back(collider);
            createGameObject = false;
        }
        else if (isTerrain)
        {
            std::shared_ptr<Geometry> terrain = it->second;
            bufferData = this->BuildBufferData(terrain);
            worldTransform = instanceGeometries[it->first]->matrix;
            for (int i = 0; i < bufferData.size(); i += 24)
            {
                glm::vec3 v1 = worldTransform * glm::vec4(bufferData[i], bufferData[i + 1], bufferData[i + 2], 1.0f);
                glm::vec3 v2 = worldTransform * glm::vec4(bufferData[i + 8], bufferData[i + 9], bufferData[i + 10], 1.f);
                glm::vec3 v3 = worldTransform * glm::vec4(bufferData[i + 16], bufferData[i + 17], bufferData[i + 18], 1.0f);
                glm::vec3 center = v1 + v2 + v3;
                center /= 3;
                glm::vec3 normal = glm::vec3(bufferData[i + 3], bufferData[i + 4], bufferData[i + 5]);
                std::string name = "collider";
                std::vector<glm::vec3> points{v1,v2,v3};
                std::shared_ptr<Triangle> collider = std::make_shared<Triangle>(Triangle(center, name, normal, points, ColliderType::TRIANGLE, DynamicType::Static, nullptr));
                objectToColliders[it->first].push_back(collider);
            }
        }
        if (createGameObject)
        {
            if (!isTerrain)
            {
                std::shared_ptr<Geometry> current = it->second;
                bufferData = this->BuildBufferData(current);
                worldTransform = instanceGeometries[it->first]->matrix;
            }
            std::pair<unsigned int, unsigned int> buffers = this->SetupBuffers(bufferData.data(), bufferData.size() * sizeof(float), false);
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(buffers.first, buffers.second, bufferData.size() / 8));
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(worldTransform, scale, rotation, translation, skew, perspective);
            rotation = glm::conjugate(rotation);
            std::shared_ptr<RigidBody> rigidBody = std::make_shared<RigidBody>(RigidBody(translation, rotation, objectToColliders[it->first]));
            std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(GameObject(texture, mesh, rigidBody, "shader", "shadowShader"));

            for (int j = 0; j < objectToColliders[it->first].size(); j++)
            {
                objectToColliders[it->first][j]->SetParent(rigidBody);
            }
            gameObjects.push_back(gameObject);
        }
    }
}

std::vector<float> ResourceManager::BuildBufferData(std::shared_ptr<Geometry> geometry)
{
    std::vector<float> bufferData;
    for (int i = 0; geometry->indices.size(); i += geometry->stride * 3)
    {
        std::vector<int> vertexIndices{ geometry->indices[i],
                                        geometry->indices[i + geometry->stride],
                                        geometry->indices[i + 2 * geometry->stride]};
        std::vector<int> textureIndices{geometry->indices[i + 2],
                                        geometry->indices[i + 2 + geometry->stride],
                                        geometry->indices[i + 2 + 2 * geometry->stride]};
        std::vector<glm::vec3> tempVertices;
        std::vector<glm::vec2> tempTextureCoords;
        for (int j = 0; j < vertexIndices.size(); j++)
        {
            int vertexIndex = vertexIndices[j] * 3;
            int textureIndex = textureIndices[j] * 2;
            float x = geometry->vertices[vertexIndex];
            float y = geometry->vertices[vertexIndex + 1];
            float z = geometry->vertices[vertexIndex + 2];

            float s = geometry->texCoords[textureIndex];
            float t = geometry->texCoords[textureIndex + 1];

            tempVertices.push_back(glm::vec3(x,y,z));
            tempTextureCoords.push_back(glm::vec2(s,t));
        }
        // Note : the cross product may point in the wrong direction.
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
    return bufferData;
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