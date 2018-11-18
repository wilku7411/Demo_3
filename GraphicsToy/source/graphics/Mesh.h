#pragma once 

#include "Shader.h"

#include <string>
#include <vector>

/*
    Have to remember that I'm using this struct in mesh setup with usage of offestof macro
    so take that under consideration in case of changes!
*/
struct SVertexData 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uvs;
};

struct STexture 
{
    unsigned int id;
    std::string type;
    std::string path;
};

struct SExtents
{
	SExtents() 
	{
		min = glm::vec3{ 0.0f, 0.0f, 0.0f };
		max = glm::vec3{ 0.0f, 0.0f, 0.0f };
	}

	SExtents(const glm::vec3& in_min, const glm::vec3& in_max)
	{
		min = in_min;
		max = in_max;
	}

	void check(const glm::vec3& min_to_check, const glm::vec3& max_to_check)
	{
		min.x = (min.x > min_to_check.x) ? min_to_check.x : min.x;
		min.y = (min.y > min_to_check.y) ? min_to_check.y : min.y;
		min.z = (min.z > min_to_check.z) ? min_to_check.z : min.z;

		max.x = (max.x < max_to_check.x) ? max_to_check.x : max.x;
		max.y = (max.y < max_to_check.y) ? max_to_check.y : max.y;
		max.z = (max.z < max_to_check.z) ? max_to_check.z : max.z;
	}

	glm::vec3 min;
	glm::vec3 max;
};

class CMesh 
{
public:
    CMesh(std::vector<SVertexData> vertices, std::vector<unsigned int> indices);
    void Draw(CShader* shader);

public:
    /*  Mesh Data  */
    std::vector<SVertexData> vertices;
    std::vector<unsigned int> indices;

	//SExtents ext;
    unsigned int vao;

private:
    unsigned int m_vertexBuffer;
    unsigned int m_elementBuffer;

    void setupMesh();
};