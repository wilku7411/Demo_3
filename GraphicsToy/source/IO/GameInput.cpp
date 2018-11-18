#include "GameInput.h"

unsigned int GameInput::keysPressed = 0;


GameInput::GameInput() 
{
}

void GameInput::Update(GLFWwindow* window)
{
	if (glfwGetKey(window, Key::UP))
	{
		keysPressed |= Action::Accelerate;
	}
	if (glfwGetKey(window, Key::DOWN))
	{
		keysPressed |= Action::Decelerate;
	}
	if (glfwGetKey(window, Key::LEFT))
	{
		keysPressed |= Action::TurnLeft;
	}
	if (glfwGetKey(window, Key::RIGHT))
	{
		keysPressed |= Action::TurnRight;
	}
	if (glfwGetKey(window, Key::SPACE))
	{
		keysPressed |= Action::Shoot;
	}
	if (glfwGetKey(window, Key::ENTER))
	{
		keysPressed |= Action::Restart;
	}
	if (glfwGetKey(window, Key::ESCAPE))
	{
		keysPressed |= Action::Exit;
	}
}

bool GameInput::IsPressed(Action actionKey)
{
	return (keysPressed & actionKey) ? true : false;
}

void GameInput::Clear()
{
	keysPressed = 0;
}
