#include <iostream>

// #include "Game.hpp"
#include "physics/Octree.hpp"

int main(int argc, char *argv[])
{
//     Game game(800,600);
//     // Start Game within Menu State
//     game.state = GAME_ACTIVE;
// //    
//     game.Run();
    Octree octree(3, glm::vec3(40.f,40.f,40.f), 40);
    std::cout << "s1" << std::endl;
    // std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(glm::vec3(25,10,25),2.f);
    octree.Insert(glm::vec3(50,51,51));
    octree.Insert(glm::vec3(50,50,52));
    std::vector<std::shared_ptr<OctreeNode>> ancestor;
    octree.CheckCollisions(octree.node, ancestor, 0);
    return 0;
}