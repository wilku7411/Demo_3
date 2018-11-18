#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/MaterialUniformUpdateWatcher.h"
#include "graphics/Model.h"
#include "graphics/Light.h"
#include "graphics/Material.h"
#include "managers/MaterialManager.h"

class Player
{
public:
    Player();

    // Initialization that requires game area borders and optional parameter
    // that indicates if borders should block the player, or loop him in game area
    void init(const glm::vec4& borders, bool looped = false);

    // Responsible for ship control 
    // W,A,S,D moves the ship
    void processInput(GLFWwindow* window, double dt);

    // Send transformations and camera info to the shader
    void update();

    // Draw player model 
    void draw();

private:
    CModel	    m_model;
    CShader     m_shader;
    CLight	    m_light;
    CMaterial   m_material;

    std::unique_ptr<CMaterialManager>               m_materialMgr;
    std::unique_ptr<CMaterialUniformUpdateWatcher>  m_watcher;

    glm::mat4 m_transform;
    glm::vec4 m_borders;
    glm::vec3 m_lastPos;

    double m_speed;
    double m_rotationSpeed;
    bool m_movedLastTick;
    bool m_reverse;
    bool m_loopedBorders;
};

