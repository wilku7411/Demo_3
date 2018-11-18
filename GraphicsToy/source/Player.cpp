#include "Player.h"

#include "MovableCamera.h"
#include "graphics\RendererVariables.h"

extern glm::vec3 g_sunPosition;

Player::Player() 
    : m_model("source/resources/meshes/rakietka_cudowna.obj")
    , m_shader("source/resources/shaders/basicMaterialVS.shader", "source/resources/shaders/basicMaterialFS.shader")
    , m_light(g_sunPosition)
    , m_material()
    , m_materialMgr(nullptr)
    , m_watcher(nullptr)
    , m_transform()
    , m_borders()
    , m_lastPos(0.0f)
    , m_speed(100.0)
    , m_rotationSpeed(10.0)
    , m_movedLastTick(false)
    , m_reverse(false)
    , m_loopedBorders(false)
{
}

void Player::init(const glm::vec4& borders, bool looped /*=false*/)
{
    m_borders = borders;
    m_loopedBorders = looped;

    m_watcher = std::make_unique<CMaterialUniformUpdateWatcher>(&m_shader);
    m_materialMgr = std::make_unique<CMaterialManager>();

    m_light.Attach(m_watcher.get());
    m_material.Attach(m_watcher.get());

    m_materialMgr->GetMaterialPrefabData("green_rubber", m_material);

    m_shader.setBool("u_blinn", true);
    m_shader.setMat4("u_model", m_transform);
    m_shader.setMat4("u_view", MovableCamera::getInstance().GetViewMatrix());
    m_shader.setMat4("u_projection", Renderer::s_MainProjectionMatrix);
}

void Player::processInput(GLFWwindow* window, double dt)
{
    if (m_movedLastTick)
    {
        m_lastPos = { m_transform[3] }; // third column of model matrix is translation

        auto x = m_transform[3].x - m_lastPos.x;
        auto z = m_transform[3].z - m_lastPos.z;

        MovableCamera::getInstance().Position.x = 0.0f;
        MovableCamera::getInstance().Position.z = m_lastPos.z + 18.0f;

        // abs step for camera height
        // y = 5      { 0   < x < 20  }
        // y = x - 15 { 20  < x < 215 }
        // y = 200    { 200 < x       }
        if (glm::abs(m_lastPos.x) < 215.0f)
            if (glm::abs(m_lastPos.x) < 20.0f)
                MovableCamera::getInstance().Position.y = 5.0f;
            else
                MovableCamera::getInstance().Position.y = glm::abs(m_lastPos).x - 15.0f;
        else
            MovableCamera::getInstance().Position.y = 200.0f;

    }

    m_movedLastTick = false;
    m_reverse = false;

    if (glfwGetKey(window, GLFW_KEY_W))
    {
        m_transform = glm::translate(m_transform, glm::vec3{ 0.0f, 0.0f, -1.0f * m_speed * dt });
        m_movedLastTick = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        m_reverse = true;
        m_transform = glm::translate(m_transform, glm::vec3{ 0.0f, 0.0f, 1.0f * m_speed * dt });
        m_movedLastTick = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        auto modifier = m_reverse ? (-1.0f) : 1.0f;
        m_transform = glm::rotate(m_transform, static_cast<float>(glm::radians(15.0) * modifier * m_rotationSpeed * dt), glm::vec3{ 0.0f, 10.0f, 0.0f });
        m_movedLastTick = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        auto modifier = m_reverse ? (-1.0f) : 1.0f;
        m_transform = glm::rotate(m_transform, static_cast<float>(glm::radians(-15.0) * modifier * m_rotationSpeed * dt), glm::vec3{ 0.0f, 10.0f, 0.0f });
        m_movedLastTick = true;
    }

    // if borders are looped, player will be spawned on the opposite wall, upon attempt to cross the border
    auto margin = 5.0f;
    if (m_loopedBorders)
    {
        if (m_transform[3][0] < m_borders[0] + margin)
        {
            m_transform[3][0] = m_borders[1] - margin;
        }
        else if (m_transform[3][0] > m_borders[1] - margin)
        {
            m_transform[3][0] = m_borders[0] + margin;
        }

        if (m_transform[3][2] < m_borders[2] + margin)
        {
            m_transform[3][2] = m_borders[3] - margin;
        }
        else if (m_transform[3][2] > m_borders[3] - margin)
        {
            m_transform[3][2] = m_borders[2] + margin;
        }
    }
    else // or else will be trapped between borders 
    {
        m_transform[3][0] = glm::clamp(m_transform[3][0], m_borders[0] + margin, m_borders[1] - margin);
        m_transform[3][2] = glm::clamp(m_transform[3][2], m_borders[2] + margin, m_borders[3] - margin);
    }
    
}

void Player::update()
{
    m_material.Update();
    m_light.Update();

    m_shader.setVec3("u_camera.position", MovableCamera::getInstance().Position);

    m_shader.setMat4("u_model", m_transform);
    m_shader.setMat4("u_view", MovableCamera::getInstance().GetViewMatrix());
}

void Player::draw()
{
    m_model.Draw(&m_shader);
}
