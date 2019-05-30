#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "External/tinyxml2.hpp"

/* 
Contains the parsed information from a <geometry> node in a collada file.
 */
class Geometry
{
    public:
        Geometry(std::string id, std::string name, int stride, std::vector<int> indices, std::vector<float> vertices, std::vector<float> texCoords);

        int stride;
        std::string id;
        std::string name;
        std::vector<int> indices;
        std::vector<float> vertices;
        std::vector<float> texCoords;
};

/* 
Contains the parsed information of a <controller> in a collada file.
 */
class Controller
{
    public:
        Controller(std::string id, std::string name, std::vector<float> indices, std::vector<float> weights);
        std::string id;
        std::string name;
        std::vector<float> indices;
        std::vector<float> weights;
};

/* 
Contains parsed information from an <animation> node in a collada file
 */
class AnimationNode
{
    public:
        AnimationNode(std::string id, std::vector<float> timeStamps, std::vector<glm::mat4> matrices);

        std::string             id;
        std::vector<float>      timeStamps;
        std::vector<glm::mat4>  matrices;
};

/* 
Contains the skeleton information of a parsed <node> node from library_visual_scenes in a collada file.
 */
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

/* 
Contains the transformation matrix of a <geometry> node in a collada file.
 */
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
        static tinyxml2::XMLElement* LoadFile(std::string filename);

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
        static std::vector<float>       BuildBufferData(std::shared_ptr<Geometry> geometry);
        static std::vector<int>         SplitStringInt(std::string& stringData);
        static std::vector<float>       SplitStringFloat(std::string& stringData);
        static std::vector<std::string> SplitString(std::string& stringData);
};