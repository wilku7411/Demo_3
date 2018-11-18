#pragma once

#include "Shader.h"
#include "../UniformChangesEvent.h"

#include <memory>

#include <map>
#include <queue>

// Suited mainly for CLight, Camera and CModel objects
class CMaterialUniformUpdateWatcher : public IUniformChangesObserver
{
public:
    using UniformDictionary = std::map<std::string, unsigned int>;

    CMaterialUniformUpdateWatcher() = delete;
    CMaterialUniformUpdateWatcher(CShader* shaderToObserve);

	// Interface implementation
	// mvp
    void UpdateMatrixModel(const glm::mat4& model)				    override;
	void UpdateMatrixView(const glm::mat4& view)				    override;
	void UpdateMatrixProjection(const glm::mat4& projection)	    override;
         
    // material
    void UpdateMaterialAmbient(const glm::vec3& material_ambient)   override;
    void UpdateMaterialDiffuse(const glm::vec3& material_diffuse)   override;
    void UpdateMaterialSpecular(const glm::vec3& material_specular) override;
    void UpdateMaterialShininess(const float shininess)             override;
    
    // light
    void UpdateLightPos(const glm::vec3& light_pos)				    override;
    void UpdateLightAmbient(const glm::vec3& light_ambient)         override;
    void UpdateLightDiffuse(const glm::vec3& light_diffuse)         override;
    void UpdateLightSpecular(const glm::vec3& light_specular)       override;
    void UpdateLightStrength(const float light_strength)            override;

    // camera
    void UpdateCameraPos(const glm::vec3& camera_pos)               override;

    // time
    void UpdateTime(const float time)                               override;

private:
	void GetUniformsLocations();

private:
	std::shared_ptr<CShader> m_shader;
    std::string              m_attachedShaderName;
                             
    UniformDictionary        m_uniforms;
                             
    std::queue<std::string>  m_updateRaports;
    std::vector<bool>        m_updatedArray;
    float                    m_counter = 0;
};

