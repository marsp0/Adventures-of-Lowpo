#include <iostream>
#include <memory>
#include "catch.hpp"
#include "../src/Systems/Input/InputSystem.hpp"
#include "../src/Components/InputComponent.hpp"
#include "../src/Entity.hpp"
#include "../src/Systems/Messaging/MouseMoveData.hpp"
#include "../src/Systems/Messaging/MoveData.hpp"

extern GLFWwindow* window;
extern float EPSILON;

TEST_CASE("Input System Test")
{
	std::unique_ptr<InputComponent> component = std::make_unique<InputComponent>();
	std::unique_ptr<Entity> entity = std::make_unique<Entity>(12);
	entity->AddComponent(std::move(component));

	std::vector<std::unique_ptr<Entity>> entities;
	entities.push_back(std::move(entity));

	std::vector<Message> messages;
	std::vector<Message> globalQueue;
	InputSystem inputSystem;
	SECTION("Testing one action")
	{
		inputSystem.actionList[Action::MoveForward] = true;
		inputSystem.generateKeyMessage=true;
		inputSystem.Update(window, entities, messages, globalQueue);
		REQUIRE(globalQueue.size() == 1);
		std::shared_ptr<MoveData> moveData = std::static_pointer_cast<MoveData>(globalQueue[0].data);
		REQUIRE(globalQueue[0].senderID == 12);
		REQUIRE(globalQueue[0].receiverID == 0);
		REQUIRE(moveData->forward == true);
	}
	SECTION("Test multiple actions")
	{
		inputSystem.actionList[Action::MoveForward] = true;
		inputSystem.actionList[Action::MoveLeft] = true;
		inputSystem.generateKeyMessage=true;
		inputSystem.Update(window, entities, messages, globalQueue);
		REQUIRE(globalQueue.size() == 1);
		std::shared_ptr<MoveData> moveData = std::static_pointer_cast<MoveData>(globalQueue[0].data);
		REQUIRE(globalQueue[0].senderID == 12);
		REQUIRE(globalQueue[0].receiverID == 0);
		REQUIRE(moveData->forward == true);
		REQUIRE(moveData->left == true);
	}
	SECTION("Test Mouse Messages")
	{
		inputSystem.generateMouseMessage = true;
		inputSystem.Update(window, entities, messages, globalQueue);
		REQUIRE(globalQueue.size() == 1);
		std::shared_ptr<MouseMoveData> mouseMoveData = std::static_pointer_cast<MouseMoveData>(globalQueue[0].data);
		REQUIRE(globalQueue[0].senderID == 12);
		REQUIRE(globalQueue[0].receiverID == 0);
		REQUIRE(mouseMoveData->deltaX - 0.f <= EPSILON);
		REQUIRE(mouseMoveData->deltaY - 0.f <= EPSILON);
	}
	SECTION("Test key action and mouse action")
	{
		inputSystem.generateMouseMessage = true;
		inputSystem.actionList[Action::MoveForward] = true;
		inputSystem.actionList[Action::MoveLeft] = true;
		inputSystem.generateKeyMessage=true;
		inputSystem.Update(window, entities, messages, globalQueue);
		REQUIRE(globalQueue.size() == 2);

		std::shared_ptr<MoveData> moveData = std::static_pointer_cast<MoveData>(globalQueue[0].data);
		REQUIRE(globalQueue[0].senderID == 12);
		REQUIRE(globalQueue[0].receiverID == 0);
		REQUIRE(moveData->forward == true);
		REQUIRE(moveData->left == true);

		std::shared_ptr<MouseMoveData> mouseMoveData = std::static_pointer_cast<MouseMoveData>(globalQueue[1].data);
		REQUIRE(globalQueue[0].senderID == 12);
		REQUIRE(globalQueue[0].receiverID == 0);
		REQUIRE(mouseMoveData->deltaX - 0.f <= EPSILON);
		REQUIRE(mouseMoveData->deltaY - 0.f <= EPSILON);
	}
}