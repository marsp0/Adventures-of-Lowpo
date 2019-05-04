#include <iostream>

#include "Game.hpp"

int main(int argc, char *argv[])
{
    Game game = Game(800,600);
    game.Run();
    return 0;
}