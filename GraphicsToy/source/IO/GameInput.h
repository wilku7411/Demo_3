#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../source/IO/Window.h"

class GameInput
{
public:
	enum Key
	{
		UP		= GLFW_KEY_UP,
		DOWN	= GLFW_KEY_DOWN,
		LEFT	= GLFW_KEY_LEFT,
		RIGHT	= GLFW_KEY_RIGHT,
		SPACE	= GLFW_KEY_SPACE,
		ENTER	= GLFW_KEY_ENTER,
		ESCAPE	= GLFW_KEY_ESCAPE,
        ZOOM_INC = GLFW_KEY_KP_ADD,
        ZOOM_DEC = GLFW_KEY_KP_SUBTRACT,
	};

	enum Action
	{
		Accelerate  = 0x0000001,
		Decelerate	= Accelerate << 1,
		TurnLeft	= Accelerate << 2,
		TurnRight	= Accelerate << 3,
		Shoot		= Accelerate << 4,
		Restart		= Accelerate << 5,
		Exit		= Accelerate << 6
	};

	GameInput();

	static void Update(GLFWwindow* window);
	static bool IsPressed(Action actionKey);
	static void Clear();

private:
	static unsigned int keysPressed;

};