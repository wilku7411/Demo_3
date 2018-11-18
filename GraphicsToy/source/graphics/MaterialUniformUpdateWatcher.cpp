#include "MaterialUniformUpdateWatcher.h"   


CMaterialUniformUpdateWatcher::CMaterialUniformUpdateWatcher(CShader* shaderToObserve)
    :
    m_shader(std::make_shared<CShader>(*shaderToObserve))
{
    m_attachedShaderName = m_shader->getName();
    GetUniformsLocations();
}

// Matrices

void CMaterialUniformUpdateWatcher::UpdateMatrixModel(const glm::mat4& model)
{
    m_shader->setMat4(m_uniforms["u_model"], model);
}

void CMaterialUniformUpdateWatcher::UpdateMatrixView(const glm::mat4& view)
{
    m_shader->setMat4(m_uniforms["u_view"], view);
}

void CMaterialUniformUpdateWatcher::UpdateMatrixProjection(const glm::mat4& projection)
{
    m_shader->setMat4(m_uniforms["u_projection"], projection);
}

// Material 

void CMaterialUniformUpdateWatcher::UpdateMaterialAmbient(const glm::vec3& material_ambient)
{
    m_shader->setVec3(m_uniforms["u_material.ambient"], material_ambient);
}

void CMaterialUniformUpdateWatcher::UpdateMaterialDiffuse(const glm::vec3& material_diffuse)
{
    m_shader->setVec3(m_uniforms["u_material.diffuse"], material_diffuse);
}

void CMaterialUniformUpdateWatcher::UpdateMaterialSpecular(const glm::vec3& material_specular)
{
    m_shader->setVec3(m_uniforms["u_material.specular"], material_specular);
}

void CMaterialUniformUpdateWatcher::UpdateMaterialShininess(const float shininess)
{
    m_shader->setFloat(m_uniforms["u_material.shininess"], shininess);
}

// Light

void CMaterialUniformUpdateWatcher::UpdateLightPos(const glm::vec3& light_pos)
{
    m_shader->setVec3(m_uniforms["u_light.position"], light_pos);
}

void CMaterialUniformUpdateWatcher::UpdateLightAmbient(const glm::vec3& light_ambient)
{
    m_shader->setVec3(m_uniforms["u_light.ambient"], light_ambient);
}

void CMaterialUniformUpdateWatcher::UpdateLightDiffuse(const glm::vec3& light_diffuse)
{
    m_shader->setVec3(m_uniforms["u_light.diffuse"], light_diffuse);
}

void CMaterialUniformUpdateWatcher::UpdateLightSpecular(const glm::vec3& light_specular)
{
    m_shader->setVec3(m_uniforms["u_light.specular"], light_specular);
}

void CMaterialUniformUpdateWatcher::UpdateLightStrength(const float light_strength)
{
    m_shader->setFloat(m_uniforms["u_light.strength"], light_strength);
}

// Camera

void CMaterialUniformUpdateWatcher::UpdateCameraPos(const glm::vec3& camera_pos)
{
    m_shader->setVec3(m_uniforms["u_camera.position"], camera_pos);
}

// Time

void CMaterialUniformUpdateWatcher::UpdateTime(const float time)
{
    m_shader->setFloat(m_uniforms["u_time"], time);
}


void CMaterialUniformUpdateWatcher::GetUniformsLocations()
{
	m_shader->use();
    unsigned int program = m_shader->getProgramID();

    // Temporary buffer for out data
    int count, size, length;                              
    unsigned int type;          
    const int bufferSize = 32;  
    char uniformName[bufferSize];

    glGetProgramiv(*m_shader, GL_ACTIVE_UNIFORMS, &count);
    printf_s("\n\nShader %s\nNumber of uniforms: %d\n", m_attachedShaderName.c_str(), count);

    for(int i = 0; i < count; i++)
    {
        glGetActiveUniform(*m_shader, i, bufferSize, &length, &size, &type, uniformName);
        m_uniforms[uniformName] = glGetUniformLocation(*m_shader, uniformName);
    }

    for (const auto& entry : m_uniforms)
    {
        std::cout << entry.first << "/" << entry.second << "\n";
    }
}
