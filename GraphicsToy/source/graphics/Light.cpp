#include "Light.h"

#include "RendererVariables.h"

CLight::CLight(const glm::vec3& light_position)     
    : m_lightPosition(light_position)
    , m_origin()
    , b_needsUpdate(true)
    , ambient(1.0f)
    , diffuse(1.0f)
    , specular(1.0f)
    , strength(1.0f)
{
}

CLight::CLight() 
    : m_lightPosition(10.f)
    , m_origin()
    , b_needsUpdate(true)
    , ambient(1.0f)
    , diffuse(1.0f)
    , specular(1.0f)
    , strength(1.0f)
{
}

void CLight::Init(const glm::mat4& viewMatrix)
{
    glm::mat4 model;
    model[3] = glm::vec4{ m_lightPosition, 1.0f };
    m_origin.Init(Renderer::s_MainProjectionMatrix, viewMatrix, model);
    b_needsUpdate = true;
}

void CLight::Update()
{
	if (b_needsUpdate)
	{
		Notify();
        m_origin.Translation(m_lightPosition);
        m_origin.Update(MovableCamera::getInstance().GetViewMatrix());
		b_needsUpdate = false;
	}
}

void CLight::DrawDebug()
{
    m_origin.Draw();
}

bool CLight::GetUpdateStatus() const
{
    return b_needsUpdate;
}

void CLight::Notify()
{
	if (m_observers.size() != 0)
	{
		for (auto sub : m_observers)
		{
			sub->UpdateLightPos(m_lightPosition);
			sub->UpdateLightAmbient(ambient);
			sub->UpdateLightDiffuse(diffuse);
			sub->UpdateLightSpecular(specular);
            sub->UpdateLightStrength(strength);
		}
	}
}

void CLight::SetLightPosition(const glm::vec3& position)
{
    m_lightPosition = position;
    b_needsUpdate = true;
}

void CLight::Ambient(glm::vec3 val)
{

    ambient = val;
    b_needsUpdate = true;
}

glm::vec3 CLight::Ambient() const
{
    return ambient;
}

void CLight::Diffuse(glm::vec3 val)
{
    diffuse = val;
    b_needsUpdate = true;
}

glm::vec3 CLight::Diffuse() const
{
    return diffuse;
}

void CLight::Specular(glm::vec3 val)
{

    specular = val;
    b_needsUpdate = true;
}

glm::vec3 CLight::Specular() const
{
    return specular;
}

void CLight::Strength(float val)
{

    strength = val;
    b_needsUpdate = true;
}

float CLight::Strength() const
{
    return strength;
}

void CLight::Origin(COriginGizmo val)
{

    m_origin = val;
}

COriginGizmo CLight::Origin() const
{
    return m_origin;
}

glm::vec3 CLight::LightPosition() const
{
    return m_lightPosition;
}
