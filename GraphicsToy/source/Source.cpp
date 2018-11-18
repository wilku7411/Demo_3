#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CTimer.h"
#include "MovableCamera.h"
#include "vao.h"

#include "IO/Window.h"

#include "graphics/RendererVariables.h"

#include "objects/Plane.h"
#include "objects/PlayerPortrait.h"
#include "objects/TerrainGrid.h"

#include "Player.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // !STB_IMAGE_IMPLEMENTATION


// used to force enable dedicated gpu usage on my laptop
#define ON_LAPTOP 0

#if ON_LAPTOP
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif // ON_LAPTOP

extern glm::vec3 g_sunPosition{ 0.0f, 100.0f, -500.0f };
extern glm::vec3 g_cameraPos{ 0.0f, 50.0f, 100.0f };
extern glm::vec3 g_lightColor{ 1.0f, 1.0f, 1.0f };

int main()
{
    CWindow mainWindow;
    mainWindow.Init("Demo", 1920, 1200, false);

    COriginGizmo worldOrigin;

    // Change this value to change the size of scene.
    const unsigned int unitGrid = 128;
    const unsigned int gridScale = 3 * unitGrid;
    // This creates a unitGrid x unitGrid 2D mesh one time. Computation is preformed at construction time.
    NGrid::CSquareGrid water{ unitGrid, 3 * gridScale, true };
    water.CreateWavesOnGrid();
    
    // Matrices that will be later used to draw the same instance of CSquareGrid but with different transformation
    glm::mat4 gridTransforms[5];
    water.GetModelMatrix(gridTransforms[0]);

    gridTransforms[1] = glm::translate(gridTransforms[1], glm::vec3{ 0.0f, static_cast<float>(3 * unitGrid), static_cast<float>(3 * unitGrid) });
    gridTransforms[1] = glm::scale(gridTransforms[1], glm::vec3{ 1.0f * gridScale,1.0f * gridScale, 1.0f });

	gridTransforms[2] = glm::translate(gridTransforms[2], glm::vec3{ 0.0f, static_cast<float>(3 * unitGrid), -static_cast<float>(3 * unitGrid) });
	gridTransforms[2] = glm::rotate(gridTransforms[2], glm::radians(180.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });
	gridTransforms[2] = glm::scale(gridTransforms[2], glm::vec3{ 1.0f * gridScale,1.0f * gridScale, 1.0f });

	gridTransforms[3] = glm::translate(gridTransforms[3], glm::vec3{ -static_cast<float>(3 * unitGrid) , static_cast<float>(3 * unitGrid), 0.0f });
	gridTransforms[3] = glm::rotate(gridTransforms[3], glm::radians(90.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });
	gridTransforms[3] = glm::scale(gridTransforms[3], glm::vec3{ 1.0f * gridScale,1.0f * gridScale, 1.0f });

	gridTransforms[4] = glm::translate(gridTransforms[4], glm::vec3{ static_cast<float>(3 * unitGrid) , static_cast<float>(3 * unitGrid) , 0.0f });
	gridTransforms[4] = glm::rotate(gridTransforms[4], glm::radians(-90.0f), glm::vec3{ 0.0f, 1.0f, 0.0f });
	gridTransforms[4] = glm::scale(gridTransforms[4], glm::vec3{ 1.0f * gridScale,1.0f * gridScale, 1.0f });

    // Simple plane with cyan_rubber material applied
    Plane ground;
    ground.Init();
    ground.ResizePlane(static_cast<float>(gridScale));
    ground.MovePlane(glm::vec3{ 0.0f, -5.0f, 0.0f });

    // Aliases
    auto& camera = MovableCamera::getInstance();
    auto& timer = CTimer::getInstance();

    Renderer::s_MainProjectionMatrix = glm::perspective(glm::radians(70.0f), (float)mainWindow.Width() / (float)mainWindow.Height(), 0.1f, 2000.0f);
    water.SetProjectionMatrix(Renderer::s_MainProjectionMatrix);

    Player player;
    player.init({
        -static_cast<float>(gridScale),
        static_cast<float>(gridScale),
        -static_cast<float>(gridScale),
        static_cast<float>(gridScale)
    }, true);
    
    ground.InitMatrices(Renderer::s_MainProjectionMatrix, g_sunPosition);
    camera.Position = g_cameraPos;

    auto previous = timer.getNow();
    auto counter = 0.0;
	while (mainWindow.IsOpen())
	{
        auto current = timer.getNow();
        auto dt = timer.toSecondsInterval(current - previous);
        
        previous = current;

        // Camera movement on xy plane
        // Camera rotation is handled by glfw mouse move callback in Window class
        if (glfwGetKey(mainWindow, GLFW_KEY_LEFT))
        {
            camera.ProcessKeyboard(Camera_Movement::LEFT, static_cast<float>(dt));
        }
        if (glfwGetKey(mainWindow, GLFW_KEY_RIGHT))
        {
            camera.ProcessKeyboard(Camera_Movement::RIGHT, static_cast<float>(dt));
        }
        if (glfwGetKey(mainWindow, GLFW_KEY_UP))
        {
            camera.ProcessKeyboard(Camera_Movement::UP, static_cast<float>(dt));
        }
        if (glfwGetKey(mainWindow, GLFW_KEY_DOWN))
        {
            camera.ProcessKeyboard(Camera_Movement::DOWN, static_cast<float>(dt));
        }
 
        player.processInput(mainWindow, dt);

        player.update();
        water.Update(camera.GetViewMatrix());
        ground.Update(camera.GetViewMatrix(), dt);
        
        counter += dt;
        if (counter > 0.016) // 60 fps
        {
            counter = 0.0;
            mainWindow.ClearUpdate();

            // this set of instructions draws the same grid multiple times
            // but will change the transformations each time, and draw type once
			water.SetDrawType(GL_TRIANGLES);
			water.SetModelMatrix(gridTransforms[0]);
            water.Draw();
			
            // cage made out of grid
			water.SetDrawType(GL_LINES);
			water.SetModelMatrix(gridTransforms[1]);
			water.Draw();
			water.SetModelMatrix(gridTransforms[2]);
			water.Draw();
			water.SetModelMatrix(gridTransforms[3]);
			water.Draw();
			water.SetModelMatrix(gridTransforms[4]);
			water.Draw();

            ground.Draw();

            player.draw();

            mainWindow.Swap();
        }
		
        mainWindow.ProcessKeyboardInput();
    } 

    mainWindow.Destroy();
    return 0;
}

