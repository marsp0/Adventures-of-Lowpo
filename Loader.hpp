#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "tinyxml2.hpp"

class Geometry
{
    public:
        Geometry(std::string name, std::vector<float> vertices, std::vector<float> normals, std::vector<float> texCoords);

        std::string name;
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texCoords;
};

class Controller
{
    public:
        Controller(std::string id, std::string name, std::vector<float> indices, std::vector<float> weights);
        std::string id;
        std::string name;
        std::vector<float> indices;
        std::vector<float> weights;
};

class Loader
{
    public:
        static void LoadFile(std::string filename);
        static std::unordered_map<std::string, std::shared_ptr<Geometry>> ParseGeometry(tinyxml2::XMLElement* libraryGeometry);
        static std::unordered_map<std::string, std::shared_ptr<Controller>> ParseControllers(tinyxml2::XMLElement* libraryControllers);
        static void ParseMaterials(tinyxml2::XMLElement* libraryMaterials);
        static void ParseVisualScenes(tinyxml2::XMLElement* libraryVisualScenes);
        static void ParseAnimations(tinyxml2::XMLElement* libraryAnimations);
        static std::vector<int> SplitStringInt(std::string& stringData);
        static std::vector<float> SplitStringFloat(std::string& stringData);
        static std::vector<std::string> SplitString(std::string& stringData);
};