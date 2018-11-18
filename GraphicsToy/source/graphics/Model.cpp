#include "Model.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include "stb_image.h"

CModel::CModel(const std::string& path, bool gamma /*= false*/)
	:
	m_gammaCorrection(gamma),
	transformOrigin(COriginGizmo()),
    m_objectColor(glm::vec3{0.1f, 0.3f, 0.1f})
{
    loadModel(path);
	m_model = glm::mat4(1.0f);
}

CModel::CModel(const CModel& other)
{
    *this = other;
}

void CModel::Update()
{
	transformOrigin.Update(m_view, m_model);
	Notify();
}

void CModel::Draw(CShader* shader)
{
    for (unsigned int i = 0; i < m_meshes.size(); i++)
        m_meshes[i].Draw(shader);
}

void CModel::DrawDebugGizmos()
{
	transformOrigin.Draw();
}

void CModel::Notify()
{
	if(m_observers.size() != 0)
	{
		for (auto sub : m_observers)
		{
			sub->UpdateMatrixModel(m_model);
			sub->UpdateMatrixView(m_view);
		}
	}
}

void CModel::loadModel(const std::string &path)
{
	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs );
 
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    
    m_directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void CModel::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
  
	for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

CMesh CModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<SVertexData> vertices;
    std::vector<unsigned int> indices;
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        SVertexData vertex;
        glm::vec3 vector; 
                          
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
		/*
		 *
		 * TODO : checking meshes vertices to update extents
		 *
		 **/

        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        
        // texture coordinates
        if (mesh->mTextureCoords[0]) 
        {
            glm::vec2 vec;
			// no support for multiple uv maps on a single model
			vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uvs = vec;
        }
        else
        {
            vertex.uvs = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    return CMesh(vertices, indices);
}