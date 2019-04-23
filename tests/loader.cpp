#include "catch.hpp"
#include <iostream>
#include "../Loader.hpp"

TEST_CASE("test geometry parsing")
{
    std::cout << "test" << std::endl;
    std::string filename = "loader_test_geometry_parser.dae";
    tinyxml2::XMLDocument document;
    document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_geometries");
    std::unordered_map<std::string, std::shared_ptr<Geometry>> result = Loader::ParseGeometry(libraryGeometries);

    REQUIRE(result.size() == 3);
}