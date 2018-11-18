#pragma once

#include <vector>
#include "glm/glm.hpp"

class IUniformChangesObserver
{
public:
    // General matrices updates
    virtual void UpdateMatrixModel(const glm::mat4& model)                  = 0;
    virtual void UpdateMatrixView(const glm::mat4& view)                    = 0;
    virtual void UpdateMatrixProjection(const glm::mat4& projection)        = 0;

    // Material updates
    virtual void UpdateMaterialAmbient(const glm::vec3& material_ambient)   = 0;
    virtual void UpdateMaterialDiffuse(const glm::vec3& material_diffuse)   = 0;
    virtual void UpdateMaterialSpecular(const glm::vec3& material_specular) = 0;
    virtual void UpdateMaterialShininess(const float shininess)             = 0;
    
    // Light updates
    virtual void UpdateLightPos(const glm::vec3& light_pos)                 = 0;
    virtual void UpdateLightAmbient(const glm::vec3& light_ambient)         = 0;
    virtual void UpdateLightDiffuse(const glm::vec3& light_diffuse)         = 0;
    virtual void UpdateLightSpecular(const glm::vec3& light_specular)       = 0;
    virtual void UpdateLightStrength(const float light_strength)            = 0;

    // Camera updates
    virtual void UpdateCameraPos(const glm::vec3& camera_pos)               = 0;

    // Time updates
    virtual void UpdateTime(const float time)                               = 0;
    
};

class IUniformAffect
{
public:
	virtual void Attach(IUniformChangesObserver* observer)
	{
		m_observers.push_back(observer);
	}

	virtual void Detach(IUniformChangesObserver* observer)
	{
		if (m_observers.size() != 0)
		{
			for (auto it = m_observers.begin(); it == m_observers.end(); it++)
			{
				if (*it == observer)
				{
					m_observers.erase(it);
				}
			}
		}
		else
		{
			printf("Error, no observers on observers list!");
		}
	}

	virtual void Notify() = 0;

protected:
	std::vector<IUniformChangesObserver*> m_observers;
};
