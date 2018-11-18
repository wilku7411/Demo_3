#include "Material.h"

CMaterial::CMaterial() 
    : ambient(1.0f)
    , diffuse(1.0f)
    , specular(1.0f)
    , shininess(1.0f)
    , b_needsUpdate(true)
{
}

void CMaterial::Update()
{
    if (b_needsUpdate)
    {
        Notify();
        b_needsUpdate = false;
    }
}

void CMaterial::Notify()
{
    if (m_observers.size() != 0)
    {
        for (auto sub : m_observers)
        {
            sub->UpdateMaterialAmbient(ambient);
            sub->UpdateMaterialDiffuse(diffuse);
            sub->UpdateMaterialSpecular(specular);
            sub->UpdateMaterialShininess(shininess);
        }
    }
}

void CMaterial::Diffuse(glm::vec3 val)
{
    diffuse = val;
    b_needsUpdate = true;
}

glm::vec3 CMaterial::Diffuse() const
{
    return diffuse;
}

void CMaterial::Specular(glm::vec3 val)
{

    specular = val;
    b_needsUpdate = true;
}

glm::vec3 CMaterial::Specular() const
{
    return specular;
}

void CMaterial::Shininess(float val)
{

    shininess = val;
    b_needsUpdate = true;
}

float CMaterial::Shininess() const
{
    return shininess;
}

void CMaterial::Ambient(glm::vec3 val)
{
    ambient = val;
    b_needsUpdate = true;
}

glm::vec3 CMaterial::Ambient() const
{
    return ambient;
}

