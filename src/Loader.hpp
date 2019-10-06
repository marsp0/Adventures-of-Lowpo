#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "External/tinyxml2.hpp"

/**
Contains the parsed information from a <geometry> node in a collada file.
 */
class Geometry
{
    public:
        Geometry(std::string id, std::string name, int stride, std::vector<int> indices, std::vector<float> vertices, std::vector<float> texCoords);

        /**
        stride indicates the amount of sections the geometry object has.

        VERTEX / NORMAL / TEX -> 3
        
        VERTEX / NORMAL -> 2
        */
        int stride;
        /**
        Identifier of the geometry object taken from the collada file.
        */
        std::string id;
        /**
        Name of the geometry object taken from the collada file.
        */
        std::string name;
        /**
        Vector that holds the indices of the object. This is used in conjunction with \b vertices and \b texCoords 
        to build the buffer data.
        */
        std::vector<int> indices;
        /**
        Vector that holds the distinct vertices present in the object. Order is not specified.
        */
        std::vector<float> vertices;
        /**
        Vector that holds the distinct tex coords present in the object. Order is not specified.
        */
        std::vector<float> texCoords;
};

/**
Contains the parsed information of a <controller> in a collada file.
 */
class Controller
{
    public:
        Controller(std::string id, std::string name, std::vector<float> indices, std::vector<float> weights);
        std::string id;
        /**
        Identifier of the controller object taken from the collada file.
        */
        std::string name;
        /**
        Vector containing the indices of the bones that are affecting each vertex in order.
        */
        std::vector<float> indices;
        /**
        Vector containing the indices of the weights that are affecting each vertex in order.
        */
        std::vector<float> weights;
};

/**
Contains parsed information from an <animation> node in a collada file
 */
class AnimationNode
{
    public:
        AnimationNode(std::string id, std::vector<float> timeStamps, std::vector<glm::mat4> matrices);
        /**
        Identifier of the bone being animated.
        */
        std::string             id;
        /**
        Time stamps of the keyframes.
        */
        std::vector<float>      timeStamps;
        /**
        Matrices of the keyframes at the given time.
        */
        std::vector<glm::mat4>  matrices;
};

/**
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

class PhysicsData
{
    public:
        std::vector<std::string>    names;
        std::vector<glm::mat4>      inertiaTensors;
        std::vector<float>          masses;
};

/*
Loader class takes care of the loading of all the data on disk.
- collada data
- physics data
*/
class Loader
{
    public:

        // Collada
        static tinyxml2::XMLElement* LoadFile(std::string filename);
        static std::shared_ptr<SkeletonNode>                                        ParseSkeletonNodes(tinyxml2::XMLElement* node);
        static std::unordered_map<std::string, std::shared_ptr<Geometry>>           ParseGeometry(tinyxml2::XMLElement* libraryGeometry);
        static std::unordered_map<std::string, std::shared_ptr<Controller>>         ParseControllers(tinyxml2::XMLElement* libraryControllers);
        static std::unordered_map<std::string, std::shared_ptr<SkeletonNode>>       ParseVisualScenesSkeletons(tinyxml2::XMLElement* libraryVisualScenes);
        static std::unordered_map<std::string, std::shared_ptr<AnimationNode>>      ParseAnimations(tinyxml2::XMLElement* libraryAnimations);
        static std::unordered_map<std::string, std::shared_ptr<InstanceGeometry>>   ParseVisualScenesStatic(tinyxml2::XMLElement* libraryVisualScenes);
        static std::unordered_map<std::string, std::shared_ptr<InstanceController>> ParseVisualScenesAnimated(tinyxml2::XMLElement* libraryVisualScenes);

        // Physics Loader
        static std::unordered_map<std::string, std::unordered_map<std::string, float>> LoadPhysicsData(std::string filename);

        // =======
        // UTILITY
        // =======
        static std::vector<float>       BuildBufferData(std::shared_ptr<Geometry> geometry);
        static std::vector<int>         SplitStringInt(std::string& stringData);
        static std::vector<float>       SplitStringFloat(std::string& stringData);
        static std::vector<std::string> SplitString(std::string& stringData);
};