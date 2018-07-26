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
#include "GameObject.hpp"


// GLFW function declerations
void key_callback(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

float deltaTime = 0.016f;

int main(int argc, char *argv[])
{

    Game game(800,600);
    // Initialize game
    game.Init();

    // DeltaTime variables
    GLfloat lastFrame = 0.0f;

    // Start Game within Menu State
    game.state = GAME_ACTIVE;

    Mesh mesh("/home/suburbanfilth/Downloads/house/house_obj.obj");
    Texture texture("/home/suburbanfilth/Downloads/house/house_diffuse.tga");
    GameObject gameobject(Transform(), std::make_shared<Texture>(texture), std::make_shared<Mesh>(mesh));
    Shader shader("vertexShader.glsl","fragmentShader.glsl");
    shader.Use();
    gameobject.transform.SetScale(glm::vec3(0.01f,0.01f,0.01f));
    game.Run();
    return 0;
}
        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,.01f,2000.f);
        // shader.SetMat4("projection", projection);

        // glm::mat4 view = camera.GetViewMatrix();
        // shader.SetMat4("view", view);

        // glm::mat4 model = gameobject.transform.getWorldMatrix();
        // gameobject.Update(deltaTime);
        // shader.SetMat4("model",model);
        // gameobject.Render();