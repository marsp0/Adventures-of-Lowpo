#include <GL/glew.h>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#include "Player.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "OBJ_Loader.hpp"
#include "tinyxml2.hpp"

ResourceManager::ResourceManager()
{
    
}

std::shared_ptr<Terrain> ResourceManager::LoadWorld(const std::string& filePath, std::unique_ptr<Scene>& scene)
{
    // texture loading
    std::shared_ptr<Texture> texture = std::make_shared<Texture>("/home/martin/Documents/Projects/Adventures-of-Lowpo/resources/color_palette.png");
    objl::Loader loader;
    loader.LoadFile(filePath);
    bool isHitbox = false;
    std::map<std::string,std::shared_ptr<Mesh>> hitboxMap;
    Material material = Material(glm::vec3(),glm::vec3(),glm::vec3(), 0.f);
    std::shared_ptr<Terrain> result;
    for (int j = 0; j < loader.LoadedMeshes.size(); j++)
    {
        if (loader.LoadedMeshes[j].MeshName == "Plane")
        {
            result = this->LoadTerrain(loader.LoadedMeshes[j].Vertices,33,8);
            result->texture = texture;
        }
        else if (loader.LoadedMeshes[j].MeshName == "Player")
        {
            this->LoadPlayer(loader.LoadedMeshes[j].Vertices, scene, texture);
        }
        else
        {
            std::vector<float> data;
            isHitbox = false;
            std::size_t found = loader.LoadedMeshes[j].MeshName.find("Hitbox");
            if (found != std::string::npos)
            {
                isHitbox = true;
            }
            if (!isHitbox)
            {
                for (int i = 0; i < loader.LoadedMeshes[j].Vertices.size(); i++)
                {
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.X);
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Y);
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].Position.Z);
                    
                    // Normals
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.X);
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Y);
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].Normal.Z);

                    // texture
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].TextureCoordinate.X);
                    data.push_back(loader.LoadedMeshes[j].Vertices[i].TextureCoordinate.Y);
                    
                }
                std::pair<unsigned int, unsigned int> buffers = this->SetupBuffers(data.data(), data.size() * sizeof(float), false);
                glm::vec3 ambient = glm::vec3(loader.LoadedMeshes[j].MeshMaterial.Ka.X, loader.LoadedMeshes[j].MeshMaterial.Ka.Y, loader.LoadedMeshes[j].MeshMaterial.Ka.Z);
                glm::vec3 diffuse = glm::vec3(loader.LoadedMeshes[j].MeshMaterial.Kd.X, loader.LoadedMeshes[j].MeshMaterial.Kd.Y, loader.LoadedMeshes[j].MeshMaterial.Kd.Z);
                glm::vec3 specular = glm::vec3(loader.LoadedMeshes[j].MeshMaterial.Ks.X, loader.LoadedMeshes[j].MeshMaterial.Ks.Y, loader.LoadedMeshes[j].MeshMaterial.Ks.Z);
                float shine = loader.LoadedMeshes[j].MeshMaterial.Ns;
                material.ambient = ambient;
                material.diffuse = diffuse;
                material.specular = specular;
                material.shine = shine;
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(buffers.first, buffers.second, loader.LoadedMeshes[j].Vertices.size()));
                hitboxMap[loader.LoadedMeshes[j].MeshName] = mesh;
            }
        }
        
    }
    for (int j = 0; j < loader.LoadedMeshes.size(); j++)
    {
        if (loader.LoadedMeshes[j].MeshName == "Plane" || loader.LoadedMeshes[j].MeshName == "Player")
        {
            continue;
        }
        isHitbox = false;
        std::size_t found = loader.LoadedMeshes[j].MeshName.find("Hitbox");
        if (found != std::string::npos)
        {
            isHitbox = true;
        }

        glm::vec3 min, max;
        

        for (int i = 0; i < loader.LoadedMeshes[j].Vertices.size(); i++)
        {
            if ((i == 0) && isHitbox)
            {
                max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

            } else if ((i != 0) && isHitbox) {
                if (max.x < loader.LoadedMeshes[j].Vertices[i].Position.X)
                    max.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                if (max.y < loader.LoadedMeshes[j].Vertices[i].Position.Y)
                    max.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                if (max.z > loader.LoadedMeshes[j].Vertices[i].Position.Z)
                    max.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;

                if (min.x > loader.LoadedMeshes[j].Vertices[i].Position.X)
                    min.x = loader.LoadedMeshes[j].Vertices[i].Position.X;
                if (min.y > loader.LoadedMeshes[j].Vertices[i].Position.Y)
                    min.y = loader.LoadedMeshes[j].Vertices[i].Position.Y;
                if (min.z < loader.LoadedMeshes[j].Vertices[i].Position.Z)
                    min.z = loader.LoadedMeshes[j].Vertices[i].Position.Z;
            }

        }
        if (isHitbox)
        {
            std::string first = loader.LoadedMeshes[j].MeshName.substr(0,loader.LoadedMeshes[j].MeshName.find("_"));
            std::string second = loader.LoadedMeshes[j].MeshName.substr(loader.LoadedMeshes[j].MeshName.find("."), 4);
            std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(GameObject(Transform(), texture, hitboxMap[first+second], PhysicsComponent(min,max, glm::vec3(0.f,0.f,0.f),ObjectType::Static),material));
            scene->gameObjects.push_back(gameObject);
        }
    }
    return result;
}

std::shared_ptr<Terrain> ResourceManager::LoadTerrain(std::vector<objl::Vertex>& vertices, int gridSize, int cellSize)
{

    // TODO (Martin) : update terrain code to use planes in a quadtree

    // build the heightmap
    std::shared_ptr<std::vector<std::vector<float>>> map = std::make_shared<std::vector<std::vector<float>>>(gridSize);
    for (int i = 0; i < gridSize; i++)
    {
        (*map)[i] = std::vector<float>(gridSize);
    }

    std::vector<float>  data;
    for (int i = 0; i < vertices.size(); i++)
    {
        int z = -((int)vertices[i].Position.Z)/cellSize;
        int x = (int)vertices[i].Position.X/cellSize;
        (*map)[z][x] = vertices[i].Position.Y;
    }   
    for (int i = 0; i < vertices.size();i++)
    {
        data.push_back(vertices[i].Position.X);
        data.push_back(vertices[i].Position.Y);
        data.push_back(vertices[i].Position.Z);

        // Normals per face
        data.push_back(vertices[i].Normal.X);
        data.push_back(vertices[i].Normal.Y);
        data.push_back(vertices[i].Normal.Z);

        // texture
        data.push_back(vertices[i].TextureCoordinate.X);
        data.push_back(vertices[i].TextureCoordinate.Y);
    }
    std::pair<unsigned int, unsigned int> buffers = this->SetupBuffers(data.data(), data.size() * sizeof(float), false);
    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(Terrain(buffers.first, buffers.second, data.size() / 6.0f, Transform(), nullptr ,map, cellSize,cellSize, position));
    return terrain;
}

void ResourceManager::LoadPlayer(std::vector<objl::Vertex>& vertices, std::unique_ptr<Scene>& scene, std::shared_ptr<Texture> texture)
{
    std::vector<float> data;
    glm::vec3 min, max;
    for (int i = 0; i < vertices.size(); i++)
    {
        if (i == 0)
        {
            max.x = vertices[i].Position.X;
            max.y = vertices[i].Position.Y;
            max.z = vertices[i].Position.Z;

            min.x = vertices[i].Position.X;
            min.y = vertices[i].Position.Y;
            min.z = vertices[i].Position.Z;

        } else if (i != 0) {
            if (max.x < vertices[i].Position.X)
                max.x = vertices[i].Position.X;
            if (max.y < vertices[i].Position.Y)
                max.y = vertices[i].Position.Y;
            if (max.z > vertices[i].Position.Z)
                max.z = vertices[i].Position.Z;

            if (min.x > vertices[i].Position.X)
                min.x = vertices[i].Position.X;
            if (min.y > vertices[i].Position.Y)
                min.y = vertices[i].Position.Y;
            if (min.z < vertices[i].Position.Z)
                min.z = vertices[i].Position.Z;
        }

        data.push_back(vertices[i].Position.X);
        data.push_back(vertices[i].Position.Y);
        data.push_back(vertices[i].Position.Z);

        data.push_back(vertices[i].Normal.X);
        data.push_back(vertices[i].Normal.Y);
        data.push_back(vertices[i].Normal.Z);

        data.push_back(vertices[i].TextureCoordinate.X);
        data.push_back(vertices[i].TextureCoordinate.Y);
    }

    std::pair<unsigned int, unsigned int> buffers = this->SetupBuffers(data.data(), data.size() * sizeof(float), false);

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh(buffers.first,buffers.second, vertices.size()));
    Material material = Material(glm::vec3(),glm::vec3(),glm::vec3(),0.f);
    std::shared_ptr<Player> player = std::make_shared<Player>(Player(Transform(), texture, mesh, PhysicsComponent(min,max, glm::vec3(0.f,-10.f,0.f) , ObjectType::Dynamic), material,(float)800, (float)600));
    std::shared_ptr<Camera> camera = player->GetCamera();
    scene->AddCamera(camera);
    scene->gameObjects.push_back(player);
    
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
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*8));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, floatCount * sizeof(float), (void*)(sizeof(float)*12));
    }
    return std::make_pair(VAO,VBO);
}

unsigned int ResourceManager::LoadTexture(const std::string& filePath)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

void ResourceManager::LoadAnimatedObject(std::string filePath)
{
    using namespace tinyxml2;
    // create and load the collada file.
    XMLDocument document;
    document.LoadFile(filePath.c_str());
    // the document represents the "whole" file so we need to query the first child which
    // is always Collada
    XMLElement* collada = document.FirstChildElement("COLLADA");

    // parse the mesh
    XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    XMLElement* geometry = libraryGeometries->FirstChildElement("geometry");
    XMLElement* mesh = geometry->FirstChildElement("mesh");

    // find name of the source holding the vertices.
    XMLElement* verticesElement = mesh->FirstChildElement("vertices");
    std::string verticesName;
    for (XMLElement* currElement = verticesElement->FirstChildElement("input"); currElement != NULL ; currElement = currElement->NextSiblingElement())
    {
        std::string semantic = currElement->Attribute("semantic");
        if (semantic == "POSITION")
        {
            verticesName = currElement->Attribute("source");
            verticesName = verticesName.substr(1,verticesName.length() - 1);
        }
    }

    std::string normalsName;
    std::string texCoordsName;
    // Find the names of the sources holding the normals and the texcoords
    XMLElement* triangles = mesh->FirstChildElement("triangles");
    for (XMLElement* currElement = triangles->FirstChildElement("input"); currElement != NULL ; currElement = currElement->NextSiblingElement("input"))
    {
        std::string semantic = currElement->Attribute("semantic");
        if (semantic == "NORMAL")
        {
            normalsName = currElement->Attribute("source");
            normalsName = normalsName.substr(1, normalsName.length() - 1);
        }
        else if (semantic == "TEXCOORD")
        {
            texCoordsName = currElement->Attribute("source");
            texCoordsName = texCoordsName.substr(1, texCoordsName.length() - 1);
        }
    }

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

    // Load the vertices/normals/texcoords into an array
    for (XMLElement* currElement = mesh->FirstChildElement("source") ; currElement != NULL ; currElement = currElement->NextSiblingElement("source"))
    {
        std::string id = currElement->Attribute("id");
        XMLElement* floatArray = currElement->FirstChildElement("float_array");
        std::string floatData = floatArray->GetText();
        if (id == verticesName)
        {
            vertices = this->SplitStringFloat(floatData);
        }
        else if (id == normalsName)
        {
            normals = this->SplitStringFloat(floatData);
        }
        else if (id == texCoordsName)
        {
            texCoords = this->SplitStringFloat(floatData);
        }
    }
    XMLElement* p = triangles->FirstChildElement("p");
    std::string pData = p->GetText();
    std::vector<int> indices = this->SplitStringInt(pData);

    XMLElement* libraryControllers = collada->FirstChildElement("library_controllers");
    XMLElement* controller = libraryControllers->FirstChildElement("controller");
    XMLElement* skin = controller->FirstChildElement("skin");
    XMLElement* joints = skin->FirstChildElement("joints");
    std::string sourceJointName;
    std::string sourceInvBindName;
    std::string sourceWeightsName;
    for (XMLElement* currElement = joints->FirstChildElement("input") ; currElement != NULL ; currElement = currElement->NextSiblingElement("input"))
    {
        std::string semantic = currElement->Attribute("semantic");
        if (semantic == "JOINT")
        {
            sourceJointName = currElement->Attribute("source");
            sourceJointName = sourceJointName.substr(1, sourceJointName.length() - 1);
        }
        else if (semantic == "INV_BIND_MATRIX")
        {
            sourceInvBindName = currElement->Attribute("source");
            sourceInvBindName = sourceInvBindName.substr(1, sourceInvBindName.length() - 1);
        }
    }

    XMLElement* vertexWeights = skin->FirstChildElement("vertex_weights");
    for (XMLElement* currElement = vertexWeights->FirstChildElement("input") ; currElement != NULL ; currElement = currElement->NextSiblingElement("input"))
    {
        std::string semantic = currElement->Attribute("semantic");
        if (semantic == "WEIGHT")
        {
            sourceWeightsName = currElement->Attribute("source");
            sourceWeightsName = sourceWeightsName.substr(1, sourceWeightsName.length() - 1);
        }
    }
    XMLElement* vCount          = vertexWeights->FirstChildElement("vcount");
    XMLElement* v               = vertexWeights->FirstChildElement("v");
    std::string vCountString    = vCount->GetText();
    std::string vString         = v->GetText();
    std::vector<int> vCountInts = this->SplitStringInt(vCountString);
    std::vector<int> vInts      = this->SplitStringInt(vString);

    std::vector<std::string>    jointNames;
    std::vector<float>          weights;
    std::vector<float>          inverseBindMatrices;
    std::map<std::string, int>  jointNameToInt;

    for (XMLElement* currElement = skin->FirstChildElement("source") ; currElement != NULL ; currElement = currElement->NextSiblingElement("source"))
    {
        std::string id = currElement->Attribute("id");
        if (id == sourceJointName)
        {
            XMLElement* nameArray = currElement->FirstChildElement("Name_array");
            std::string nameArrayData = nameArray->GetText();
            jointNames = this->SplitString(nameArrayData);
            for (int i = 0; i < jointNames.size() ; i++)
            {
                jointNameToInt[jointNames[i]] = i;
            }
        }
        else if (id == sourceInvBindName)
        {
            XMLElement* floatArray = currElement->FirstChildElement("float_array");
            std::string floatArrayData = floatArray->GetText();
            inverseBindMatrices = this->SplitStringFloat(floatArrayData);
        }
        else if (id == sourceWeightsName)
        {
            XMLElement* floatArray = currElement->FirstChildElement("float_array");
            std::string floatArrayData = floatArray->GetText();
            weights = this->SplitStringFloat(floatArrayData);
        }
    }
    
    //
    std::vector<int> arrayOfSums(vCountInts.size());
    int sumToHere = 0;
    for (int i = 0; i < vCountInts.size(); i++)
    {
        arrayOfSums[i] = sumToHere;
        sumToHere += vCountInts[i] * 2;
    }

    std::vector<float> bufferData;
    for (int i = 0; i < indices.size() ; i += 12)
    {
        int v1Index = indices[i];
        int v2Index = indices[i+4];
        int v3Index = indices[i+8];
        std::vector<int> vIndices{v1Index,v2Index,v3Index};

        int v1TexIndex = indices[i+2];
        int v2TexIndex = indices[i+6];
        int v3TexIndex = indices[i+10];
        std::vector<int> vTexIndices{v1TexIndex,v2TexIndex,v3TexIndex};

        glm::vec3 vertex1 = glm::vec3(vertices[v1Index*3], vertices[v1Index*3 + 1], vertices[v1Index*3 + 2]);
        glm::vec3 vertex2 = glm::vec3(vertices[v2Index*3], vertices[v2Index*3 + 1], vertices[v2Index*3 + 2]);
        glm::vec3 vertex3 = glm::vec3(vertices[v3Index*3], vertices[v3Index*3 + 1], vertices[v3Index*3 + 2]);
        
        glm::vec3 triangleNormal = glm::cross(vertex2-vertex1, vertex3-vertex1);

        for (int i = 0; i < vIndices.size(); i++)
        {
            int currentIndex = vIndices[i];
            int currentTexIndex = vTexIndices[i];
            glm::vec3 vertex = glm::vec3(vertices[currentIndex*3], vertices[currentIndex*3 + 1], vertices[currentIndex*3 + 2]);
            glm::vec2 texCoord = glm::vec2(texCoords[currentTexIndex*2], texCoords[currentTexIndex*2 + 1]);

            std::map<int, float> boneToWeightMap;
            std::vector<int> boneIndices;
            std::vector<float> weightValues;

            for (int j = arrayOfSums[currentIndex]; j < arrayOfSums[currentIndex] + vCountInts[currentIndex] * 2; j += 2)
            {
                boneToWeightMap[vInts[j]] = weights[vInts[j+1]];   
                weightValues.push_back(weights[vInts[j+1]]);
            }
            if (weightValues.size() > 4)
            {
                std::sort(weightValues.begin(), weightValues.end());
                std::reverse(weightValues.begin() , weightValues.end());
                weightValues.resize(4);
            }
            for (int j = 0; j < weightValues.size() ; j++)
            {
                for (std::map<int,float>::iterator k = boneToWeightMap.begin(); k != boneToWeightMap.end() ; k++)
                {
                    if (weightValues[j] == k->second)
                    {
                        boneIndices.push_back(k->first);
                        break;
                    }
                }
            }

            float sumToNormalize = 0;
            float factor = 0;
            for (int j = 0; j < weightValues.size() ; j++)
            {
                sumToNormalize += weightValues[j];
            }
            factor = 1.0f/sumToNormalize;
            for (int j = 0; j < weightValues.size() ; j++)
            {
                weightValues[j] *= factor;
            }
            if (weightValues.size() < 4)
            {
                for (int j = 0; j < 4 - weightValues.size(); j++ )
                {
                    weightValues.push_back(0.f);
                    boneIndices.push_back(0);
                }
            }
            // Vertex data
            bufferData.push_back(vertex.x);
            bufferData.push_back(vertex.y);
            bufferData.push_back(vertex.z);

            bufferData.push_back(triangleNormal.x);
            bufferData.push_back(triangleNormal.y);
            bufferData.push_back(triangleNormal.z);

            bufferData.push_back(texCoord.x);
            bufferData.push_back(texCoord.y);

            bufferData.push_back((float)boneIndices[0]);
            bufferData.push_back((float)boneIndices[1]);
            bufferData.push_back((float)boneIndices[2]);
            bufferData.push_back((float)boneIndices[3]);

            bufferData.push_back(weightValues[0]);
            bufferData.push_back(weightValues[1]);
            bufferData.push_back(weightValues[2]);
            bufferData.push_back(weightValues[3]);
        }
    }

    // Load bone structure

    XMLElement* libraryVisualScenes = collada->FirstChildElement("library_visual_scenes");
    XMLElement* visualScene = libraryVisualScenes->FirstChildElement("visual_scene");

    std::vector<std::shared_ptr<Bone>> bonesVector;
    std::shared_ptr<BoneTreeNode> root;

    for (XMLElement* currNode = visualScene->FirstChildElement("node") ; currNode != NULL; currNode = currNode->NextSiblingElement("node"))
    {
        std::string id = currNode->Attribute("id");
        if (id == "Armature")
        {
            XMLElement* torsoNode = currNode->FirstChildElement("node");
            root = this->ParseNode(torsoNode,bonesVector,glm::mat4(1.0));
        }
    }

}

std::shared_ptr<BoneTreeNode> ResourceManager::ParseNode(tinyxml2::XMLElement* node, std::vector<std::shared_ptr<Bone>>& bonesVector, glm::mat4 parentOffset)
{
    using namespace tinyxml2;
    int     boneIndex           = bonesVector.size();
    std::string name            = node->Attribute("sid");
    XMLElement* matrixNode      = node->FirstChildElement("matrix");
    std::string matrixNodeData  = matrixNode->GetText();
    std::vector<float> matrixFloats = this->SplitStringFloat(matrixNodeData);
    // just to not lose track of this -> https://math.stackexchange.com/questions/688339/product-of-inverse-matrices-ab-1
    glm::mat4 offsetMat         = glm::inverse(glm::make_mat4(matrixFloats.data())) * parentOffset;
    std::shared_ptr<Bone> bone  = std::make_shared<Bone>(boneIndex, offsetMat, name);
    bonesVector.push_back(bone);

    std::shared_ptr<BoneTreeNode> btn = std::make_shared<BoneTreeNode>();
    btn->boneIndex = boneIndex;
    for (XMLElement* currChild = node->FirstChildElement("node"); currChild != NULL; currChild = currChild->NextSiblingElement("node"))
    {
        btn->children.push_back(this->ParseNode(currChild, bonesVector, bone->GetOffsetMatrix()));
    }
    return btn;
}


std::vector<float> ResourceManager::SplitStringFloat(const std::string& stringData)
{
    std::vector<float> result;
    size_t prevDelimeter = 0;
    size_t currentDelimeter = stringData.find(" ");
    while (currentDelimeter != std::string::npos)
    {
        result.push_back(std::stof(stringData.substr(prevDelimeter, currentDelimeter - prevDelimeter)));
        prevDelimeter = currentDelimeter + 1;
        currentDelimeter = stringData.find(" ", prevDelimeter);
    }
    if (prevDelimeter != currentDelimeter)
    {
        result.push_back(std::stof(stringData.substr(prevDelimeter, currentDelimeter)));
    }
    return result;
}

std::vector<int> ResourceManager::SplitStringInt(const std::string& stringData)
{
    std::vector<int> result;
    size_t prevDelimeter = 0;
    size_t currentDelimeter = stringData.find(" ");
    while (currentDelimeter != std::string::npos)
    {
        result.push_back(std::stoi(stringData.substr(prevDelimeter, currentDelimeter - prevDelimeter)));    
        prevDelimeter = currentDelimeter + 1;
        currentDelimeter = stringData.find(" ", prevDelimeter);
    }
    if (prevDelimeter != currentDelimeter)
    {
        result.push_back(std::stoi(stringData.substr(prevDelimeter, currentDelimeter)));
    }
    return result;
}

std::vector<std::string> ResourceManager::SplitString(const std::string& stringData)
{
    std::vector<std::string> result;
    size_t prevDelimeter = 0;
    size_t currentDelimeter = stringData.find(" ");
    while (currentDelimeter != std::string::npos)
    {
        result.push_back(stringData.substr(prevDelimeter, currentDelimeter - prevDelimeter));
        prevDelimeter = currentDelimeter + 1;
        currentDelimeter = stringData.find(" ", prevDelimeter);
    }
    if (prevDelimeter != currentDelimeter)
    {
        result.push_back(stringData.substr(prevDelimeter, currentDelimeter));
    }
    return result;
}