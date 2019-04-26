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

void Loader::LoadFile(std::string filename)
{
    XMLDocument document;
    // TODO : check for the extension and report error if different from .dae
    XMLError error = document.LoadFile(filename.c_str());
    if (error != 0)
        return;
    // the document represents the "whole" file so we need to qu
    // is always Collada
    XMLElement* collada = document.FirstChildElement("COLLADA");
    XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    std::unordered_map<std::string, std::shared_ptr<Geometry>> geometries = Loader::ParseGeometry(libraryGeometries);
}

std::unordered_map<std::string, std::shared_ptr<Geometry>> Loader::ParseGeometry(XMLElement* libraryGeometries)
{
    std::unordered_map<std::string, std::shared_ptr<Geometry>> result;
    for (XMLElement* current = libraryGeometries->FirstChildElement("geometry"); current != NULL ; current = current->NextSiblingElement("geometry"))
    {
        std::string name = current->Attribute("name");
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
                std::string offset = input->Attribute("offset");
                texCoordsOffset = std::stoi(offset);
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
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
        for (int i = 0; i < indices.size(); i += 3 * stride)
        {
            int v1Index = indices[i];
            int v2Index = indices[i + stride];
            int v3Index = indices[i + 2 * stride];
            std::vector<int> vIndices{v1Index,v2Index,v3Index};

            int v1TexIndex = indices[i + texCoordsOffset];
            int v2TexIndex = indices[i + stride + texCoordsOffset];
            int v3TexIndex = indices[i + 2 * stride + texCoordsOffset];
            std::vector<int> vTexIndices{v1TexIndex,v2TexIndex,v3TexIndex};

            glm::vec3 vertex1 = glm::vec3(vertexValues[v1Index*3], vertexValues[v1Index*3 + 1], vertexValues[v1Index*3 + 2]);
            glm::vec3 vertex2 = glm::vec3(vertexValues[v2Index*3], vertexValues[v2Index*3 + 1], vertexValues[v2Index*3 + 2]);
            glm::vec3 vertex3 = glm::vec3(vertexValues[v3Index*3], vertexValues[v3Index*3 + 1], vertexValues[v3Index*3 + 2]);
            
            // calculate normal
            glm::vec3 triangleNormal = glm::cross(vertex2-vertex1, vertex3-vertex1);
            for (int j = 0; j < vIndices.size(); j++)
            {
                int currentIndex = vIndices[j];
                int currentTexIndex = vTexIndices[j];
                glm::vec3 vertex = glm::vec3(vertexValues[currentIndex*3], vertexValues[currentIndex*3 + 1], vertexValues[currentIndex*3 + 2]);
                glm::vec2 texCoord = glm::vec2(texCoordValues[currentTexIndex*2], texCoordValues[currentTexIndex*2 + 1]);
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
                normals.push_back(triangleNormal.x);
                normals.push_back(triangleNormal.y);
                normals.push_back(triangleNormal.z);
                texCoords.push_back(texCoord.x);
                texCoords.push_back(texCoord.y);
            }
        }
        std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>(Geometry(name, vertices, normals, texCoords));
        result[name] = geometry;
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

// =================
// UTILITY FUNCTIONS
// =================
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

Geometry::Geometry( std::string name, 
                    std::vector<float> vertices, 
                    std::vector<float> normals, 
                    std::vector<float> texCoords) : \
                    name(name),
                    vertices(vertices),
                    normals(normals),
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