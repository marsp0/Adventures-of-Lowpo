#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"

Game::Game(unsigned int width, unsigned int height) :
    width(width), height(height)
{
    this->deltaTime = 0.016f;
}

Game::~Game()
{

}

void Game::Init()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create window
    this->window = glfwCreateWindow(this->width, this->height, "Shootemup", nullptr, nullptr);
    glfwMakeContextCurrent(this->window);

    // Disable mouse within window
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
    glewInit();
    glGetError();
    glViewport(0, 0, this->width, this->height);
}

void Game::HandleInput(float deltaTime)
{
    // KEYBOARD
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,FORWARD);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,BACKWARD);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,LEFT);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        this->scene->camera.HandleKeyboardInput(deltaTime,RIGHT);

    // MOUSE
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);
    float xoffset = xpos - this->lastPositionX;
    float yoffset = this->lastPositionY - ypos;

    this->lastPositionX = xpos;
    this->lastPositionY = ypos;

    this->scene->camera.HandleMouseInput(deltaTime,xoffset,yoffset);

}

void Game::Update(float deltaTime)
{

}

void Game::Render()
{
    
}

void Game::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle Player Input
        this->HandleInput(deltaTime);
        // Update Game state
        this->Update(deltaTime);
        // Render
        this->Render();

        // std::cout << glGetError() << std::endl;
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}