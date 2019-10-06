#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Loader.hpp"

using namespace tinyxml2;

// Geometry Constants
const std::string NORMAL        = "NORMAL";
const std::string POSITION      = "POSITION";
const std::string TEXCOORD      = "TEXCOORD";
// Animation Constants
const std::string JOINT         = "JOINT";
const std::string INVBINDMAT    = "INV_BIND_MATRIX";
const std::string WEIGHT        = "WEIGHT";
const std::string ANIMATION_INPUT = "INPUT";
const std::string ANIMATION_OUTPUT = "OUTPUT";

XMLElement* Loader::LoadFile(std::string filename)
{
    XMLDocument document;
    // TODO : check for the extension and report error if different from .dae
    XMLError error = document.LoadFile(filename.c_str());
    if (error != 0)
        return nullptr;
    // the document represents the "whole" file so we need to qu
    // is always Collada
    XMLElement* collada = document.FirstChildElement("COLLADA");
    return collada;
}

std::unordered_map<std::string, std::shared_ptr<Geometry>> Loader::ParseGeometry(XMLElement* libraryGeometries)
{
    // geometryID should be the same as the id of node in library_visual_scenes.
    std::unordered_map<std::string, std::shared_ptr<Geometry>> result;
    for (XMLElement* current = libraryGeometries->FirstChildElement("geometry"); current != NULL ; current = current->NextSiblingElement("geometry"))
    {
        std::string name = current->Attribute("name");
        std::string geometryID = current->Attribute("id");
        geometryID = geometryID.substr(0, geometryID.find("-mesh"));
        XMLElement* mesh = current->FirstChildElement("mesh");
        XMLElement* verticesNode = mesh->FirstChildElement("vertices");
        
        std::string verticesID;
        std::string texCoordsID;
        for (XMLElement* input = verticesNode->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == POSITION)
            {
                verticesID = input->Attribute("source");
                verticesID = verticesID.substr(1, verticesID.length() - 1);
            }
        }
        XMLElement* trianglesNode = mesh->FirstChildElement("triangles");
        
        int stride = 0;
        int texCoordsOffset;
        for (XMLElement* input = trianglesNode->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == TEXCOORD)
            {
                texCoordsID = input->Attribute("source");
                texCoordsID = texCoordsID.substr(1, texCoordsID.length() - 1);
            }
            stride++;
        }
        
        std::vector<float> vertexValues;
        std::vector<float> texCoordValues;
        for (XMLElement* currElement = mesh->FirstChildElement("source") ; currElement != NULL ; currElement = currElement->NextSiblingElement("source"))
        {
            std::string id = currElement->Attribute("id");
            XMLElement* floatArray = currElement->FirstChildElement("float_array");
            std::string floatData = floatArray->GetText();
            if (id == verticesID)
            {
                vertexValues = Loader::SplitStringFloat(floatData);
            }
            else if (id == texCoordsID)
            {
                texCoordValues = Loader::SplitStringFloat(floatData);
            }
        }
        XMLElement* p = trianglesNode->FirstChildElement("p");
        std::string pData = p->GetText();
        std::vector<int> indices = Loader::SplitStringInt(pData);
        std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>(Geometry(geometryID, name, stride, indices, vertexValues, texCoordValues));
        result[geometryID] = geometry;
    }
    return result;
}

std::unordered_map<std::string, std::shared_ptr<Controller>> Loader::ParseControllers(XMLElement* libraryControllers)
{
    std::unordered_map<std::string, std::shared_ptr<Controller>> result;
    for (XMLElement* controller = libraryControllers->FirstChildElement("controller"); controller != NULL; controller = controller->NextSiblingElement("controller"))
    {
        XMLElement* skin = controller->FirstChildElement("skin");
        XMLElement* joints = skin->FirstChildElement("joints");

        // the joints node contains input nodes that point to the right sources
        // for the matrices and the joints.
        std::string sourceJoint;
        std::string sourceInvBind;
        std::string sourceWeights;
        for (XMLElement* input = joints->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == JOINT)
            {
                sourceJoint = input->Attribute("source");
                sourceJoint = sourceJoint.substr(1, sourceJoint.size() - 1);
            }
            else if (semantic == INVBINDMAT)
            {
                sourceInvBind = input->Attribute("source");
                sourceInvBind = sourceInvBind.substr(1, sourceInvBind.size() - 1);
            }
        }
        XMLElement* vertexWeights = skin->FirstChildElement("vertex_weights");
        for (XMLElement* input = vertexWeights->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == WEIGHT)
            {
                sourceWeights = input->Attribute("source");
                sourceWeights = sourceWeights.substr(1, sourceWeights.size() - 1);
            }
        }
        XMLElement* vCount          = vertexWeights->FirstChildElement("vcount");
        XMLElement* v               = vertexWeights->FirstChildElement("v");

        std::string vString         = v->GetText();
        std::string vCountString    = vCount->GetText();
        std::vector<int> vCountInts = Loader::SplitStringInt(vCountString);
        std::vector<int> vInts      = Loader::SplitStringInt(vString);

        std::vector<std::string>    jointNames;
        std::vector<float>          weights;
        std::vector<float>          inverseBindMatricesFloats;
        std::vector<glm::mat4>          inverseBindMatrices;
        std::unordered_map<std::string, int> jonitNameToInt;
        for (XMLElement* source = skin->FirstChildElement("source"); source != NULL; source = source->NextSiblingElement("source"))
        {
            std::string id = source->Attribute("id");
            if (id == sourceJoint)
            {
                std::string nameArrayString = source->FirstChildElement("Name_array")->GetText();
                jointNames = Loader::SplitString(nameArrayString);
                for (int i = 0; i < jointNames.size(); i++)
                {
                    jonitNameToInt[jointNames[i]] = i;
                }
            }
            else if (id == sourceInvBind)
            {
                std::string floatArrayString = source->FirstChildElement("float_array")->GetText();
                inverseBindMatricesFloats = Loader::SplitStringFloat(floatArrayString);
                for (int i = 0; i < inverseBindMatricesFloats.size() ; i += 16)
                {
                    std::vector<float> temp;
                    for (int j = i; j < i + 16; j++)
                    {
                        temp.push_back(inverseBindMatricesFloats[j]);
                    }
                    inverseBindMatrices.push_back(glm::transpose(glm::make_mat4(temp.data())));
                }
            }
            else if (id == sourceWeights)
            {
                std::string floatArrayString = source->FirstChildElement("float_array")->GetText();
                weights = Loader::SplitStringFloat(floatArrayString);
            }
        }
        std::vector<int> arrayOfSums(vCountInts.size());
        int sumToHere = 0;
        for (int i = 0; i < arrayOfSums.size(); i++)
        {
            arrayOfSums[i] = sumToHere;
            sumToHere += vCountInts[i] * 2;
        }
        std::vector<float> finalIndices;
        std::vector<float> finalWeights;

        for (int i = 0; i < vCountInts.size(); i++)
        {
            int vIndex                      = arrayOfSums[i];
            std::vector<int>                tempIndexValues;
            std::vector<float>              tempWeights;
            std::unordered_map<int, float>  tempIndexToWeight;

            for (int j = vIndex; j < vIndex + vCountInts[i] * 2; j += 2)
            {
                int boneIndex = vInts[j];
                float weightIndex = vInts[j + 1];
                tempIndexToWeight[boneIndex] = weights[weightIndex];
                tempWeights.push_back(weights[weightIndex]);
            }
            if (tempWeights.size() > 4)
            {
                std::sort(tempWeights.begin(), tempWeights.end());
                std::reverse(tempWeights.begin(), tempWeights.end());
                tempWeights.resize(4);
            }
            for (int x = 0; x < tempWeights.size(); x++)
            {
                for (std::unordered_map<int,float>::iterator k = tempIndexToWeight.begin(); k != tempIndexToWeight.end(); k++)
                {
                    if (tempWeights[x] == k->second)
                    {
                        tempIndexValues.push_back(k->first);
                        break;
                    }
                }
            }
            float sumToNormalize = 0.f;
            float factor = 0.f;
            for (int x = 0; x < tempWeights.size(); x++)
            {
                sumToNormalize += tempWeights[x];
            }
            factor = 1.0f/sumToNormalize;
            for (int x = 0; x < tempWeights.size(); x++)
            {
                tempWeights[x] *= factor;
            }
            if (tempWeights.size() < 4)
            {
                int toPush = 4 - tempWeights.size();
                for (int x = 0; x < toPush; x++)
                {
                    tempWeights.push_back(0.f);
                    tempIndexValues.push_back(0.f);
                }
            }
            for (int x = 0; x < tempIndexValues.size(); x++)
            {
                finalIndices.push_back(tempIndexValues[x]);
                finalWeights.push_back(tempWeights[x]);
            }
        }
        std::string id = controller->Attribute("id");
        std::string name = controller->Attribute("name");
        result[name] = std::make_shared<Controller>(Controller(id, name, finalIndices, finalWeights));
    }
    return result;
}

std::unordered_map<std::string, std::shared_ptr<AnimationNode>> Loader::ParseAnimations(XMLElement* libraryAnimations)
{
    std::unordered_map<std::string, std::shared_ptr<AnimationNode>> result;
    for (XMLElement* animation = libraryAnimations->FirstChildElement("animation"); animation != NULL; animation = animation->NextSiblingElement("animation"))
    {
        std::string             inputID;
        std::string             outputID;
        std::vector<float>      timeStamps;
        std::vector<glm::mat4>  matrices;
        XMLElement* sampler = animation->FirstChildElement("sampler");
        for (XMLElement* input = sampler->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == ANIMATION_INPUT)
            {
                inputID = input->Attribute("source");
                inputID = inputID.substr(1, inputID.size() - 1);
            }
            else if (semantic == ANIMATION_OUTPUT)
            {
                outputID = input->Attribute("source");
                outputID = outputID.substr(1, outputID.size() - 1);
            }
        }
        for (XMLElement* source = animation->FirstChildElement("source"); source != NULL; source = source->NextSiblingElement("source"))
        {
            std::string id = source->Attribute("id");
            if (id == inputID)
            {
                std::string timeStampsString = source->FirstChildElement("float_array")->GetText();
                timeStamps = Loader::SplitStringFloat(timeStampsString);
            }
            else if (id == outputID)
            {
                std::string matricesString = source->FirstChildElement("float_array")->GetText();
                std::vector<float> matricesFloat = Loader::SplitStringFloat(matricesString);
                for (int i = 0; i < matricesFloat.size(); i += 16)
                {
                    std::vector<float> temp;
                    for (int j = i; j < i + 16; j++)
                    {
                        temp.push_back(matricesFloat[j]);
                    }
                    matrices.push_back(glm::transpose(glm::make_mat4(temp.data())));
                }
            }
        }
        std::string id = animation->Attribute("id");
        result[id] = std::make_shared<AnimationNode>(AnimationNode(id, timeStamps, matrices));
    }
    return result;
}

std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>> Loader::ParseVisualScenesStatic(XMLElement* libraryVisualScenes)
{
    std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>> result;
    XMLElement* visualScene = libraryVisualScenes->FirstChildElement("visual_scene");
    for (XMLElement* node = visualScene->FirstChildElement("node"); node != NULL; node = node->NextSiblingElement("node"))
    {
        XMLElement* instanceGeometry = node->FirstChildElement("instance_geometry");
        if (instanceGeometry != nullptr)
        {
            std::string id = instanceGeometry->Attribute("url");
            id = id.substr(1, id.find("-mesh") - 1);
            std::string name = instanceGeometry->Attribute("name");
            XMLElement* matrixNode = node->FirstChildElement("matrix");
            std::string matrixString = matrixNode->GetText();
            glm::mat4 matrix = glm::transpose(glm::make_mat4(Loader::SplitStringFloat(matrixString).data()));
            std::shared_ptr<InstanceGeometry> instance = std::make_shared<InstanceGeometry>(InstanceGeometry(id, name, matrix));
            result[id] = instance;
        }
    }
    return result;
}

std::unordered_map<std::string, std::shared_ptr<InstanceController>> Loader::ParseVisualScenesAnimated(XMLElement* libraryVisualScenes)
{
    std::unordered_map<std::string, std::shared_ptr<InstanceController>> result;
    XMLElement* visualScene = libraryVisualScenes->FirstChildElement("visual_scene");
    for (XMLElement* node = visualScene->FirstChildElement("node"); node != NULL; node = node->NextSiblingElement("node"))
    {
        XMLElement* instanceController = node->FirstChildElement("instance_controller");
        if (instanceController != nullptr)
        {
            std::string id = node->Attribute("id");
            std::string name = node->Attribute("name");
            std::string url = instanceController->Attribute("url");
            url = url.substr(1, url.size() - 1);
            std::shared_ptr<InstanceController> instance = std::make_shared<InstanceController>(InstanceController(id, name, url));
            result[id] = instance;
        }
    }
    return result;
}

std::unordered_map<std::string, std::shared_ptr<SkeletonNode>> Loader::ParseVisualScenesSkeletons(XMLElement* libraryVisualScenes)
{
    std::unordered_map<std::string, std::shared_ptr<SkeletonNode>> result;
    XMLElement* visualScene = libraryVisualScenes->FirstChildElement("visual_scene");
    for (XMLElement* node = visualScene->FirstChildElement("node"); node != NULL; node = node->NextSiblingElement("node"))
    {
        XMLElement* instanceGeometry = node->FirstChildElement("instance_geometry");
        XMLElement* instanceControllers = node->FirstChildElement("instance_controller");
        if (instanceControllers == nullptr && instanceGeometry == nullptr)
        {
            std::string id = node->Attribute("id");
            std::string name = node->Attribute("name");
            std::string sid = "";
            std::string matrixString = node->FirstChildElement("matrix")->GetText();
            glm::mat4 matrix = glm::transpose(glm::make_mat4(Loader::SplitStringFloat(matrixString).data()));
            std::vector<std::shared_ptr<SkeletonNode>> children;
            for (XMLElement* child = node->FirstChildElement("node"); child != NULL; child = child->NextSiblingElement("node"))
            {
                std::shared_ptr<SkeletonNode> childNode = Loader::ParseSkeletonNodes(child);
                children.push_back(childNode);
            }
            std::shared_ptr<SkeletonNode> skeleton = std::make_shared<SkeletonNode>(SkeletonNode(id, name, sid, matrix, children));
            result[id] = skeleton;  
        }
    }
    return result;
}

std::shared_ptr<SkeletonNode> Loader::ParseSkeletonNodes(tinyxml2::XMLElement* node)
{
    std::string id = node->Attribute("id");
    std::string name = node->Attribute("name");
    std::string sid = node->Attribute("sid");
    std::string matrixString = node->FirstChildElement("matrix")->GetText();
    glm::mat4 matrix = glm::transpose(glm::make_mat4(Loader::SplitStringFloat(matrixString).data()));
    std::vector<std::shared_ptr<SkeletonNode>> children;
    for (XMLElement* child = node->FirstChildElement("node"); child != NULL; child = child->NextSiblingElement("node"))
    {
        std::shared_ptr<SkeletonNode> childNode = Loader::ParseSkeletonNodes(child);
        children.push_back(childNode);
    }
    std::shared_ptr<SkeletonNode> skeleton = std::make_shared<SkeletonNode>(SkeletonNode(id, name, sid, matrix, children));
    return skeleton;
}

// =================
// UTILITY FUNCTIONS
// =================

std::vector<float> Loader::BuildBufferData(std::shared_ptr<Geometry> geometry)
{
    std::vector<float> bufferData;
    for (int i = 0; i < geometry->indices.size(); i += geometry->stride * 3)
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

std::vector<float> Loader::SplitStringFloat(std::string& stringData)
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

std::vector<int> Loader::SplitStringInt(std::string& stringData)
{
    std::vector<int> result;
    size_t prevDelimeter = 0;
    size_t currentDelimeter = stringData.find(" ");
    std::string testString = " ";
    // there is a space before the end tag </vcount> and it only happens there.
    if (stringData[stringData.size() - 1] == testString[0])
        stringData = stringData.substr(0,stringData.size() - 1);
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

std::vector<std::string> Loader::SplitString(std::string& stringData)
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


// ====================================================
// Library Classes
// ====================================================

Geometry::Geometry( std::string id,
                    std::string name, 
                    int stride,
                    std::vector<int> indices,
                    std::vector<float> vertices, 
                    std::vector<float> texCoords) : \
                    id(id),
                    name(name),
                    stride(stride),
                    indices(indices),
                    vertices(vertices),
                    texCoords(texCoords)
{

}

Controller::Controller(std::string id,
                        std::string name,
                        std::vector<float> indices,
                        std::vector<float> weights) : \
                        id(id),
                        name(name),
                        indices(indices),
                        weights(weights)
{

}

AnimationNode::AnimationNode(   std::string id, 
                                std::vector<float> timeStamps, 
                                std::vector<glm::mat4> matrices) : \
                                id(id),
                                timeStamps(timeStamps),
                                matrices(matrices)
{
    
}

InstanceGeometry::InstanceGeometry( std::string id,
                                    std::string name,
                                    glm::mat4 matrix) : \
                                    id(id),
                                    name(name),
                                    matrix(matrix)
{

}

InstanceController::InstanceController( std::string id,
                                        std::string name,
                                        std::string url) : \
                                        id(id),
                                        name(name),
                                        url(url)
{

}

SkeletonNode::SkeletonNode( std::string id, 
                            std::string name, 
                            std::string sid,
                            glm::mat4 matrix, 
                            std::vector<std::shared_ptr<SkeletonNode>> children) : \
                            id(id),
                            name(name),
                            sid(sid),
                            matrix(matrix),
                            children(children)
{

}

// PHYSICS

std::unordered_map<std::string, std::unordered_map<std::string, float>> Loader::LoadPhysicsData(std::string filename)
{
    const std::string MASS = "mass";
    const std::string INERTIA = "inertia";
    std::unordered_map<std::string, std::unordered_map<std::string, float>> result;
    std::ifstream fileStream(filename);
    std::string line;
    std::string current;
    while (std::getline(fileStream, line))
    {
        std::vector<std::string> tokens = Loader::SplitString(line);
        if (tokens.size() == 1)
        {
            current = tokens[0];
            result[current] = std::unordered_map<std::string, float>{};
        }
        else
            result[current][tokens[0]] = std::stof(tokens[1]);
    }
    fileStream.close();
    return result;
}