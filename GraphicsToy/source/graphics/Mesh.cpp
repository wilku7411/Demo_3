#include "Mesh.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <cstddef>
#include <fstream>
#include <sstream>
#include <iostream>

    CMesh::CMesh(std::vector<SVertexData> vertices, std::vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
		
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &m_vertexBuffer);
		glGenBuffers(1, &m_elementBuffer);

        setupMesh();
    }

    // render the mesh
    void CMesh::Draw(CShader* shader)
    {
		shader->use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
    }

    void CMesh::setupMesh() 
    {
		glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertexData), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (void*)offsetof(SVertexData, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (void*)offsetof(SVertexData, uvs));
    
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    
        glBindVertexArray(0);
    }