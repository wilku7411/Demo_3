#pragma once


#include "../UniformChangesEvent.h"
#include "../MovableCamera.h"
#include "OriginGizmo.h"

#include "glm/glm.hpp"


class CLight : public IUniformAffect
{
public:
    CLight();
    CLight(const glm::vec3& light_position);

    void Init(const glm::mat4& viewMatrix);
	void Update();

    void DrawDebug();

	bool GetUpdateStatus() const;

	// Interface implementation
	virtual void Notify() override;

public:
    COriginGizmo Origin() const;
	glm::vec3 LightPosition() const;	
    glm::vec3 Ambient() const;
    glm::vec3 Diffuse() const;
    glm::vec3 Specular() const;
    float Strength() const;

    void Origin(COriginGizmo val);
	void SetLightPosition(const glm::vec3& position);
    void Ambient(glm::vec3 val);
    void Diffuse(glm::vec3 val);
    void Specular(glm::vec3 val);
    void Strength(float val);

private:
	glm::vec3       m_lightPosition;
    COriginGizmo    m_origin;

	bool            b_needsUpdate;
    glm::vec3       ambient;
    glm::vec3       diffuse;
    glm::vec3       specular;
    float           strength;
};