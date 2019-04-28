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
        Geometry(std::string name, std::vector<int> indices, std::vector<float> vertices, std::vector<float> texCoords);

        std::string name;
        std::vector<int> indices;
        std::vector<float> vertices;
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

class AnimationNode
{
    public:
        AnimationNode(std::string id, std::vector<float> timeStamps, std::vector<glm::mat4> matrices);

        std::string             id;
        std::vector<float>      timeStamps;
        std::vector<glm::mat4>  matrices;
};

class SkeletonNode
{
    public:
        SkeletonNode(std::string id, std::string name, std::string sid, glm::mat4 matrix, std::vector<std::shared_ptr<SkeletonNode>> children);

        std::string id;
        std::string sid;
        std::string name;
        glm::mat4   matrix;
        std::vector<std::shared_ptr<SkeletonNode>> children;
};

class InstanceGeometry
{
    public:
        InstanceGeometry(std::string id, std::string name, glm::mat4 matrix);

        std::string id;
        std::string name;
        glm::mat4   matrix;
};

class InstanceController
{
    public:
        InstanceController(std::string id, std::string name, std::string url);

        std::string id;
        std::string name;
        std::string url;
};

class Loader
{
    public:
        static void LoadFile(std::string filename);

        static std::shared_ptr<SkeletonNode>                                        ParseSkeletonNodes(tinyxml2::XMLElement* node);
        static std::unordered_map<std::string, std::shared_ptr<Geometry>>           ParseGeometry(tinyxml2::XMLElement* libraryGeometry);
        static std::unordered_map<std::string, std::shared_ptr<Controller>>         ParseControllers(tinyxml2::XMLElement* libraryControllers);
        static std::unordered_map<std::string, std::shared_ptr<SkeletonNode>>       ParseVisualScenesSkeletons(tinyxml2::XMLElement* libraryVisualScenes);
        static std::unordered_map<std::string, std::shared_ptr<AnimationNode>>      ParseAnimations(tinyxml2::XMLElement* libraryAnimations);
        static std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>>   ParseVisualScenesStatic(tinyxml2::XMLElement* libraryVisualScenes);
        static std::unordered_map<std::string, std::shared_ptr<InstanceController>> ParseVisualScenesAnimated(tinyxml2::XMLElement* libraryVisualScenes);
        // =======
        // UTILITY
        // =======
        static std::vector<int>         SplitStringInt(std::string& stringData);
        static std::vector<float>       SplitStringFloat(std::string& stringData);
        static std::vector<std::string> SplitString(std::string& stringData);
};