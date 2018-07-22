#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Game.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Texture.hpp"


// GLFW function declerations
void key_callback(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
Camera camera;
float deltaTime = 0.016f;
double lastPosX = 400.f;
double lastPosY = 300.f;

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
    glewInit();
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

    // glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glEnable(GL_DEPTH_TEST);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize game
    game.Init();

    // DeltaTime variables
    GLfloat lastFrame = 0.0f;

    // Start Game within Menu State
    game.state = GAME_ACTIVE;

    Mesh mesh("/home/suburbanfilth/Downloads/house/house_obj.obj");
    Texture texture("/home/suburbanfilth/Downloads/house/house_spec.tga");
    texture.Bind();
    Shader shader("vertexShader.glsl","fragmentShader.glsl");
    shader.Use();

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        // GLfloat currentFrame = glfwGetTime();
        // deltaTime = currentFrame - lastFrame;
        // lastFrame = currentFrame;
        glfwPollEvents();

        //deltaTime = 0.001f;
        // Manage user input
        // game.HandleInput(deltaTime);

        // Update Game state
        // game.Update(deltaTime);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // game.Render();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,.1f,2000.f);
        shader.SetMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.SetMat4("view", view);

        glm::mat4 model;
        model = glm::translate(model,glm::vec3(0.0f,0.0f,-3.0f));
        model = glm::scale(model,glm::vec3(0.1f,0.1f,0.1f));
        shader.SetMat4("model",model);
        mesh.Draw();
        // std::cout << glGetError() << std::endl;
        glfwSwapBuffers(window);
        key_callback(window);

    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application  
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // if (key >= 0 && key < 1024)
    // {
    //     if (action == GLFW_PRESS)
    //         game.keys[key] = GL_TRUE;
    //     else if (action == GLFW_RELEASE)
    //         game.keys[key] = GL_FALSE;
    // }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.HandleKeyboardInput(deltaTime,FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.HandleKeyboardInput(deltaTime,BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.HandleKeyboardInput(deltaTime,LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.HandleKeyboardInput(deltaTime,RIGHT);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastPosX;
    float yoffset = lastPosY - ypos;

    lastPosX = xpos;
    lastPosY = ypos;

    camera.HandleMouseInput(deltaTime,xoffset,yoffset);
}