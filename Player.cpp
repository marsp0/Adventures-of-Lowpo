#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "Player.hpp"

Player::Player( Transform                   transform,
                std::shared_ptr<Texture>    texture,
                std::shared_ptr<Mesh>       mesh,
                std::shared_ptr<Animator>   animator,
                PhysicsComponent physicsComponent,
                Material material,
                float cameraWidth,
                float cameraHeight) :
                GameObject(transform, texture, mesh,physicsComponent, material)
                
{
    this->direction = glm::vec3(0.f,0.f,-1.f);
    this->speed = 10.5f;
    this->physicsComponent.position = glm::vec3(10.f,0.f,-12.f);
    this->physicsComponent.boundingBox.Update(glm::vec3(10.f,0.f,-12.f));
    this->camera = std::make_shared<Camera>(Camera(cameraWidth, cameraHeight, glm::vec3(0.f,0.f, 50.f), glm::vec3(0.f,0.f,0.f)));
    this->animator = animator;
}

void Player::HandleInput(GLFWwindow* window)
{
    // KEYBOARD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->actions[MOVE_FOREWARD] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        this->actions[MOVE_BACKWARD] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->actions[MOVE_LEFT] = true;
        
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->actions[MOVE_RIGHT] = true;
    }
    this->camera->HandleInput(window);
}

void Player::Update(float deltaTime)
{
    this->transform.SetPosition(this->physicsComponent.GetPosition());
    // NOTE : Implement diagonal walk
    // Try : temp vector and add the different velocities before setting their total
    // to the physics component

    // The 0 velocity check is at the beginning because
    // otherwise we cant move ; dont move the code.
    if (!this->actions[MOVE_FOREWARD] && !this->actions[MOVE_BACKWARD] \
        && !this->actions[MOVE_LEFT] && !this->actions[MOVE_RIGHT])
    {
        this->SetVelocity(glm::vec3(0.f,0.f,0.f));
    }
    if (this->actions[MOVE_FOREWARD])
    {
        this->actions[MOVE_FOREWARD] = false;
        glm::vec3 velocity(this->direction*this->speed);
        this->SetVelocity(velocity);
    }
    if (this->actions[MOVE_BACKWARD])
    {
        this->actions[MOVE_BACKWARD] = false;
        this->SetVelocity(-this->direction*this->speed);
    }
    if (this->actions[MOVE_LEFT])
    {
        this->actions[MOVE_LEFT] = false;
        this->SetVelocity(glm::rotate(this->direction,glm::radians(90.f), this->camera->worldUp) * this->speed);
    }
    if (this->actions[MOVE_RIGHT])
    {
        this->actions[MOVE_RIGHT] = false;
        this->SetVelocity(glm::rotate(this->direction * this->speed,glm::radians(-90.f),this->camera->worldUp));
    }
    this->camera->Update(this);
    this->direction = glm::normalize(this->camera->direction);

    // Update Animations
    this->animator->Update(deltaTime);
}

std::shared_ptr<Camera> Player::GetCamera()
{
    return this->camera;
}

void Player::Render(Shader& shader)
{
    // bind offsets and transforms
    std::vector<glm::mat4> offsets = this->animator->skeleton->GetOffsetMatrices();
    std::vector<glm::mat4> anims = this->animator->skeleton->GetAnimMatrices();
    for (int i = 0; i < offsets.size() ; i++)
    {
        std::string gBones = "gBones[" + std::to_string(i) + "]";
        std::string gBinds = "offsets[" + std::to_string(i) + "]";
        shader.SetMat4(gBones.c_str(), anims[i]);
        shader.SetMat4(gBinds.c_str(), offsets[i]);
    }
    GameObject::Render(shader);
}