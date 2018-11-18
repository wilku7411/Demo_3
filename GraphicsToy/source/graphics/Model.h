#pragma once

#include "Mesh.h"
#include "OriginGizmo.h"
#include "../UniformChangesEvent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

class CModel : public IUniformAffect
{
public:
    CModel() = default;
    CModel(const CModel& other);
    CModel(const std::string& path, bool gammaCorrection = false);
	
    virtual ~CModel() = default;

	void Update();

	void Draw(CShader* shader);
	void DrawDebugGizmos();

	glm::mat4 GetModelMat() const			  { return m_model; };
    glm::mat4 GetViewMat() const			  { return m_view; };
	glm::vec3 GetObjectColor() const		  { return m_objectColor; }
    
	void SetModelMat(const glm::mat4& matrix) { m_model = matrix; };
    void SetViewMat(const glm::mat4& matrix)  { m_view = matrix; };
	void SetObjectColor(glm::vec3 val)		  { m_objectColor = val; }
	
	// Interface implementation
	void Notify() override;

public:
	COriginGizmo transformOrigin;

private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    CMesh processMesh(aiMesh *mesh, const aiScene *scene);

private:
    std::vector<STexture> m_textures;
    std::vector<CMesh> m_meshes;

	glm::vec3 m_objectColor;
	glm::mat4 m_view, m_model;

    std::string m_directory;
    bool m_gammaCorrection;
};