#include <iostream>

#include "Game.hpp"

int main(int argc, char *argv[])
{
    Game game(800,600);
    // Start Game within Menu State
    game.state = GAME_ACTIVE;
//    
    game.Run();
    return 0;
}