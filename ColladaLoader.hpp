#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// Very simple and slow Collada loader. DO NOT USE

namespace Collada
{
    class Input
    {
        public:
            Input(std::string semantic, std::string source);
            std::string semantic;
            std::string source;
    };

    class Vertices
    {
        public:
            Vertices(std::string ID, Input input);
            Input       input;
            std::string ID;
    };

    class Param
    {
        public:
            Param(std::string name);
            std::string name;
    };

    class Accessor
    {
        public:
            Accessor(std::string source, int count, int stride, std::vector<Param> params);
            std::string         source;
            int                 count;
            int                 stride;
            std::vector<Param>  params;
    };

    class TechniqueCommon
    {
        public:
            TechniqueCommon(Accessor accessor);
            Accessor accessor;
    };

    class Triangles
    {
        public:
            Triangles(std::string material, std::vector<Input> inputs);
            std::vector<Input>  inputs;
            std::string         material;
    };

    class Source
    {
        public:
            Source(bool isFloatArray, TechniqueCommon techniqueCommon, std::vector<float> floatArray, std::vector<std::string> stringArray, std::string id);

            bool                        isFloatArray;
            std::string                 id;
            TechniqueCommon             techniqueCommon;
            std::vector<float>          floatArray;
            std::vector<std::string>    stringArray;
    };

    class GeometryLibrary
    {
        // this is going to hold the mesh data
        // positions/normals/texture coords
        public:

            GeometryLibrary(std::string geometryData);

            Source positions;
            Source normals;
            Source texCoords;
    };

    // library_animations
    class Animations
    {

    };

    // library_controllers
    class AnimationControllers
    {

    };

    // library_visual_scenes
    class AnimationVisualScenes
    {

    };

    class Loader
    {
        public:
            Loader(std::string filename);
            std::vector<std::string> SplitString(std::string stringData);
            GeometryLibrary ParseGeometry(std::string geometryString);
            Source          ParseSource(const std::string& sourceData);
            Triangles       ParseTriangles(std::string trianglesData);
            TechniqueCommon ParseTechnique(std::string techniqueData);
            Accessor        ParseAccessor(std::string accessorData);
            Param           ParseParam(std::string paramData);
            Vertices        ParseVertices(std::string verticesData);
            Input           ParseInput(std::string inputData);

            std::string colladaStringFile;
            std::shared_ptr<GeometryLibrary> geometry;
        private:

    };
};