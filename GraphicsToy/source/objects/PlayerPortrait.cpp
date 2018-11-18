#include "PlayerPortrait.h"



CPlayerPortrait::CPlayerPortrait() 
    : m_transform()
    , m_camera()
    , m_projection()
    , m_shader(std::make_unique<CShader>("source/resources/shaders/mmVS.shader", "source/resources/shaders/mmFS.shader"))
{
    for (int i = 0; i < 2; i++)
        m_textures.push_back(CTexture{ "source/resources/textures" });

    Init();
}

void CPlayerPortrait::Init()
{
    m_quad.createQuad();
    AddTexture();
    m_shader->setInt(glGetUniformLocation(m_shader->getProgramID(), "texSampler"), 0);

    m_shader->setMat4("u_model", m_transform);
    m_shader->setMat4("u_view", m_camera);
    m_shader->setMat4("u_projection", m_projection);
}

void CPlayerPortrait::Draw()
{
    m_shader->use();

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(m_quad.vertexArray);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    m_shader->disable();
}

void CPlayerPortrait::AddTexture()
{
    m_textures[0].CreateTexture2D("pepe.png");
    m_textures[1].CreateTexture2D("bg.png");

    for (int i = 0; i < 2; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        m_textures[i].Use();
        std::string textureUniformName = "u_texture" + std::to_string(i);
        glUniform1i(glGetUniformLocation(m_shader->getProgramID(), textureUniformName.c_str()), i);
    }
}