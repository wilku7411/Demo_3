#pragma once

#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../vao.h"

#include <memory>

class CPlayerPortrait
{
public:
    CPlayerPortrait();
    void Init();
    void Draw();
    void AddTexture();

private:
    nshapes::vao			 m_quad;
                             
    glm::mat4				 m_transform;
    glm::mat4				 m_camera;
    glm::mat4				 m_projection;

    std::unique_ptr<CShader> m_shader;
    std::vector<CTexture>	 m_textures;
};