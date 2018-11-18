#include "Plane.h"

#include "../graphics/Primitives.h"
#include "../graphics/Shader.h"


Plane::Plane()
    : m_materialManager(std::make_shared<CMaterialManager>())
    , m_shader(std::make_unique<CShader>("source/resources/shaders/basicMaterialVS.shader", "source/resources/shaders/basicMaterialFS.shader"))
{
}

void Plane::Init()
{
    m_isInstanced = false;

    m_observer = std::make_unique<CMaterialUniformUpdateWatcher>(m_shader.get());
    m_lightSource.Attach(m_observer.get());
    m_material.Attach(m_observer.get());

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(2, m_buffers);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NPrimitives::planeVertexData.size(), NPrimitives::planeVertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NPrimitives::planeIndices.size(), NPrimitives::planeIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Plane::InitInstanced(const unsigned int tileMapSize, int density /*= 2*/)
{
    m_isInstanced = true;

    m_observer = std::make_unique<CMaterialUniformUpdateWatcher>(m_shader.get());
    m_lightSource.Attach(m_observer.get());
    m_material.Attach(m_observer.get());

    CreateInstancesTransforms(tileMapSize, density);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(3, m_buffers);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NPrimitives::planeVertexData.size(), NPrimitives::planeVertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NPrimitives::planeIndices.size(), NPrimitives::planeIndices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_instancesTransforms.size(), m_instancesTransforms.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[2]);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
    glBindVertexArray(0);
}

void Plane::InitMatrices(const glm::mat4& projection, const glm::vec3& lightPosition)
{
    m_projection = projection;
    m_lightSource.SetLightPosition(lightPosition);
    m_lightSource.Init(MovableCamera::getInstance().GetViewMatrix());

    m_shader->setMat4(glGetUniformLocation(*m_shader, "u_model"), m_modelMat);
    m_shader->setMat4(glGetUniformLocation(*m_shader, "u_projection"), m_projection);

    m_materialManager->GetMaterialPrefabData("cyan_rubber", m_material);

    m_shader->setBool(glGetUniformLocation(*m_shader, "u_blinn"), true);
    m_shader->setVec3("u_camera.position", MovableCamera::getInstance().Position);
}

void Plane::Update(const glm::mat4& viewMatrix, const float dt)
{
    m_gizmo.UpdateMatrices(m_projection, viewMatrix, m_modelMat);

    m_shader->setMat4(glGetUniformLocation(*m_shader, "u_view"), viewMatrix);

    // Camera
    m_shader->setVec3("u_camera.position", MovableCamera::getInstance().Position);

    // Material 
    m_material.Update();
    m_lightSource.Update();

    m_observer->UpdateTime(m_planeTimer);
}

void Plane::Draw()
{
    m_shader->use();
    glBindVertexArray(m_vao);
    
    switch (m_isInstanced)
    {
    case true:
        glDrawElementsInstanced(GL_TRIANGLES, NPrimitives::planeIndices.size(), GL_UNSIGNED_INT, 0, m_instancesTransforms.size());
        break;

    case false:
        glDrawElements(GL_TRIANGLES, NPrimitives::planeIndices.size(), GL_UNSIGNED_INT, 0);
        break;

    default:
        break;
    }

    glBindVertexArray(0);
    m_shader->disable();

    m_lightSource.DrawDebug();
    m_gizmo.Draw();
}

void Plane::ResizePlane(const float newSize)
{
    const auto y = m_modelMat[3][1];

    m_modelMat = glm::mat4{ 1.0f };
    m_modelMat = glm::translate(m_modelMat, glm::vec3{ -newSize, y, -newSize });
    m_modelMat = glm::scale(m_modelMat, glm::vec3{ newSize, 1.0f, newSize });
    m_shader->setMat4(glGetUniformLocation(*m_shader, "u_model"), m_modelMat);
}

void Plane::MovePlane(const glm::vec3& translation)
{
    m_modelMat = glm::translate(m_modelMat, translation);
    m_shader->setMat4(glGetUniformLocation(*m_shader, "u_model"), m_modelMat);
}

void Plane::RotatePlane(const glm::vec3& axis, const float angle)
{
    m_modelMat = glm::rotate(m_modelMat, angle, axis);
    m_shader->setMat4(glGetUniformLocation(*m_shader, "u_model"), m_modelMat);
}

void Plane::CreateInstancesTransforms(const unsigned int tilesExtents, int density)
{
    const int& extents = tilesExtents;

    for (int z = -extents; z < extents; z += density)
    {
        for (int x = -extents; x < extents; x += density)
        {
            glm::vec3 translation;
            translation.x = (float)x;
            translation.y = 0.0f;
            translation.z = (float)z;
            m_instancesTransforms.push_back(translation);
        }
    }
}
