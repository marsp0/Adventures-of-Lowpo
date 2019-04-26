#include <iostream>

#include "Game.hpp"
#include "Loader.hpp"

int main(int argc, char *argv[])
{
    std::string filename = "./test/files/controllers.dae";
    tinyxml2::XMLDocument document;
    tinyxml2::XMLError error = document.LoadFile(filename.c_str());
    tinyxml2::XMLElement* collada = document.FirstChildElement("COLLADA");
    tinyxml2::XMLElement* libraryGeometries = collada->FirstChildElement("library_animations");
    std::unordered_map<std::string, std::shared_ptr<AnimationNode>> result = Loader::ParseAnimations(libraryGeometries);
    std::cout << result.size() << std::endl;
    return 0;
}