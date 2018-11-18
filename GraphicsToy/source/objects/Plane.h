#pragma once

#include "../graphics/Light.h"
#include "../graphics/Material.h"
#include "../graphics/MaterialUniformUpdateWatcher.h"
#include "../graphics/OriginGizmo.h"
#include "../graphics/Texture.h"
#include "../UniformChangesEvent.h"
#include "../vao.h"

#include "../managers/MaterialManager.h"

#include "../MovableCamera.h"
#include "../CTimer.h"

#include <array>
#include <memory>

class Plane 
{
public:
    Plane();

    void Init();
    void InitInstanced(const unsigned int tileMapSize, int density = 2);
	void InitMatrices(const glm::mat4& projection, const glm::vec3& lightPosition);
	void Update(const glm::mat4& viewMatrix, const float dt);
	void Draw();

    void ResizePlane(const float newSize);
    void MovePlane(const glm::vec3& translation);
    void RotatePlane(const glm::vec3& axis, const float angle);

protected:
    void CreateInstancesTransforms(const unsigned int tilesExtents, int density);

protected:
    std::unique_ptr<CMaterialUniformUpdateWatcher> m_observer;
	
    std::vector<glm::vec3>		      m_instancesTransforms;
	glm::mat4					      m_modelMat, m_projection;
	std::vector<unsigned int>	      m_texIds;
                                      
    std::unique_ptr<CShader>          m_shader;
    std::shared_ptr<CMaterialManager> m_materialManager;
    CMaterial                         m_material;
    CLight						      m_lightSource;            

    COriginGizmo				      m_gizmo;
                                      
    double                            m_planeTimer;
                                      
    unsigned int				      m_vao;
	unsigned int				      m_buffers[3];
    bool                              m_isInstanced;
};