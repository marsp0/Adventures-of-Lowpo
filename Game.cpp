#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game.hpp"
#include <memory>

Game::Game(int width, int height) :
    width(width), height(height)
{
    this->Init();
    this->deltaTime = 0.016f;
    this->scene = std::make_unique<Scene>(Scene((float)this->width, (float)this->height));
    this->renderer = std::make_unique<Renderer>(Renderer("vertexShader.glsl","fragmentShader.glsl"));
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Mesh("/home/suburbanfilth/Downloads/house/house_obj.obj"));
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture("/home/suburbanfilth/Downloads/house/house_diffuse.tga"));
    std::shared_ptr<GameObject> gameobject = std::make_shared<GameObject>(GameObject(Transform(), texture,mesh));
    gameobject->transform.Scale(glm::vec3(.1f,.1f,.1f));
    this->scene->gameObjects.push_back(gameobject);
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
    this->renderer->Draw(this->scene);
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
        glfwSwapBuffers(window);
        unsigned int error = glGetError();
        if (error != 0)
        {
            std::cout << error << std::endl;
        }

    }

    glfwTerminate();
}