#include "catch.hpp"
#include <iostream>
#include <unistd.h>
#include "../Loader.hpp"

TEST_CASE("Geometry parsing")
{
    std::string filename = "./test/files/loader_test_geometry_parser.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    std::unordered_map<std::string, std::shared_ptr<Geometry>> result = Loader::ParseGeometry(libraryGeometries);
    // parsed 3 objects
    REQUIRE(result.size() == 3);

    // Plane tests
    std::shared_ptr<Geometry> plane = result["Plane"];
    REQUIRE(plane->name == "Plane");
    REQUIRE(plane->indices.size() == 18);
    REQUIRE(plane->vertices.size() == 12);
    REQUIRE(plane->texCoords.size() == 12);

    // Plane tests
    std::shared_ptr<Geometry> cube = result["Cube.000"];
    REQUIRE(cube->name == "Cube.000");
    REQUIRE(cube->indices.size() == 108);
    REQUIRE(cube->vertices.size() == 24);
    REQUIRE(cube->texCoords.size() == 72);

    // Plane tests
    std::shared_ptr<Geometry> cube1 = result["Cube.001"];
    REQUIRE(cube1->name == "Cube.001");
    REQUIRE(cube1->indices.size() == 108);
    REQUIRE(cube1->vertices.size() == 24);
    REQUIRE(cube1->texCoords.size() == 72);
}

TEST_CASE("Controller parsing")
{
    std::string filename = "./test/files/controllers.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryControllers = collada->FirstChildElement("library_controllers");
    std::unordered_map<std::string, std::shared_ptr<Controller>> result = Loader::ParseControllers(libraryControllers);
    REQUIRE(result.size() == 1);

    std::shared_ptr<Controller> controller = result["Armature"];
    REQUIRE(controller->id == "Armature_Cube_001-skin");
    REQUIRE(controller->name == "Armature");
    REQUIRE(controller->indices.size() == 740 * 4);
    REQUIRE(controller->weights.size() == 740 * 4);
}

TEST_CASE("Animations parsing")
{
    std::string filename = "./test/files/controllers.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryAnimations = collada->FirstChildElement("library_animations");
    std::unordered_map<std::string, std::shared_ptr<AnimationNode>> result = Loader::ParseAnimations(libraryAnimations);

    REQUIRE(result.size() == 21);
    for (std::unordered_map<std::string, std::shared_ptr<AnimationNode>>::iterator it = result.begin(); it != result.end(); it++)
    {
        std::shared_ptr<AnimationNode> animation = it->second;
        REQUIRE(animation->id.size() > 0);
        REQUIRE(animation->matrices.size() == 21);
        REQUIRE(animation->timeStamps.size() == 21);
    }
}