#pragma once

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
    public:
        Game(unsigned int width, unsigned int height);
        ~Game();

        void Init();
        void HandleInput(float deltaTime);
        void Update(float deltaTime);
        void Render();

        // data
        GameState state;
        bool keys[1024];
        unsigned int width, height;
};