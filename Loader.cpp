#include "Loader.hpp"

const std::string NORMAL = "NORMAL";
const std::string POSITION = "POSITION";
const std::string TEXCOORD = "TEXCOORD";

void Loader::LoadFile(std::string filename)
{
    tinyxml2::XMLDocument document;
    // TODO : check for the extension and report error if different from .dae
    document.LoadFile(filename.c_str());
    // the document represents the "whole" file so we need to qu
    // is always Collada
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    std::unordered_map<std::string, std::shared_ptr<Geometry>> geometries = Loader::ParseGeometry(libraryGeometries);
}

std::unordered_map<std::string, std::shared_ptr<Geometry>> Loader::ParseGeometry(tinyxml2::XMLElement* libraryGeometries)
{
    std::unordered_map<std::string, std::shared_ptr<Geometry>> result;
    for (tinyxml2::XMLElement* current = libraryGeometries->FirstChildElement("geometry"); current != NULL ; current = current->NextSiblingElement("geometry"))
    {
        std::string name = current->Attribute("name");
        tinyxml2::XMLElement* mesh = current->FirstChildElement("mesh");
        tinyxml2::XMLElement* verticesNode = mesh->FirstChildElement("vertices");
        std::string verticesID;
        std::string normalsID;
        std::string texCoordsID;
        for (tinyxml2::XMLElement* input = verticesNode->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == POSITION)
            {
                verticesID = input->Attribute("source");
                verticesID = verticesID.substr(1, verticesID.length() - 1);
            }
        }
        tinyxml2::XMLElement* trianglesNode = mesh->FirstChildElement("triangles");
        for (tinyxml2::XMLElement* input = trianglesNode->FirstChildElement("input"); input != NULL; input = input->NextSiblingElement("input"))
        {
            std::string semantic = input->Attribute("semantic");
            if (semantic == NORMAL)
            {
                normalsID = input->Attribute("source");
                normalsID = normalsID.substr(1, normalsID.length() - 1);
            }
            else if (semantic == TEXCOORD)
            {
                texCoordsID = input->Attribute("source");
                texCoordsID = texCoordsID.substr(1, texCoordsID.length() - 1);
            }
        }
        std::vector<float> vertexValues;
        std::vector<float> normalValues;
        std::vector<float> texCoordValues;
        for (tinyxml2::XMLElement* currElement = mesh->FirstChildElement("source") ; currElement != NULL ; currElement = currElement->NextSiblingElement("source"))
        {
            std::string id = currElement->Attribute("id");
            tinyxml2::XMLElement* floatArray = currElement->FirstChildElement("float_array");
            std::string floatData = floatArray->GetText();
            if (id == verticesID)
            {
                vertexValues = Loader::SplitStringFloat(floatData);
            }
            else if (id == normalsID)
            {
                vertexValues = Loader::SplitStringFloat(floatData);
            }
            else if (id == texCoordsID)
            {
                texCoordValues = Loader::SplitStringFloat(floatData);
            }
        }
        tinyxml2::XMLElement* p = trianglesNode->FirstChildElement("p");
        std::string pData = p->GetText();
        std::vector<int> indices = Loader::SplitStringInt(pData);

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
        for (int i = 0; i < indices.size(); i++)
        {
            int v1Index = indices[i];
            int v2Index = indices[i+4];
            int v3Index = indices[i+8];
            std::vector<int> vIndices{v1Index,v2Index,v3Index};

            int v1TexIndex = indices[i+2];
            int v2TexIndex = indices[i+6];
            int v3TexIndex = indices[i+10];
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
        std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>(name, vertices, normals, texCoords);
        result[name] = geometry;
    }
    return result;
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
// Geometry
// ====================================================

Geometry::Geometry( std::string name, 
                    std::vector<float> vertices, 
                    std::vector<float> normals, 
                    std::vector<float> textureCoordinates) : \
                    name(name),
                    vertices(vertices),
                    normals(normals),
                    texCoords(texCoords)
{

}