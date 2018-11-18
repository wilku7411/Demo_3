#include "TerrainGrid.h"

#include "../vao.h"
#include "../MovableCamera.h"
#include "../CTimer.h"

extern glm::vec3 g_sunPosition;

namespace NGrid
{

	CSquareGrid::CSquareGrid(const unsigned int res, const unsigned int scaleMultiplier /*= 1*/, bool eraseFromRAM /*= true*/)
		: m_resolution(res)
		, m_drawable(false)
		, m_transformGizmo()
		, m_shader(std::make_unique<CShader>("source/resources/shaders/terrainVS.shader", "source/resources/shaders/terrainFS.shader"))
		, m_eraseFromRAM(eraseFromRAM)
		, m_hasWaves(false)
	{
		m_vertices.reserve((m_resolution + 1)*(m_resolution + 1));
		m_indices.reserve(6 * (m_resolution + 1)*(m_resolution + 1));

		GenerateGrid();
		m_modelMat = glm::translate(m_modelMat, glm::vec3{ 0.0f, -20.0f , 0.0f });
		m_modelMat = glm::rotate(m_modelMat, glm::radians(-90.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });
		m_modelMat = glm::scale(m_modelMat, glm::vec3{ 1.0f * scaleMultiplier,1.0f * scaleMultiplier , 1.0f });
	}

	void CSquareGrid::GenerateGrid()
	{
		GenerateVertices();
		GenerateIndices();
		DrawInit();
	}

	void CSquareGrid::SetDrawType(unsigned int type)
	{
		m_drawType = type;
	}

    void CSquareGrid::SetProjectionMatrix(const glm::mat4& projection)
    {
        m_shader->setMat4(glGetUniformLocation(*m_shader, "u_projection"), projection);
        m_transformGizmo.UpdateMatrices(projection, glm::mat4{ 1.0f }, glm::mat4{ 1.0f });
    }

	void CSquareGrid::GetModelMatrix(glm::mat4& out_mat)
	{
		out_mat = m_modelMat;
	}

	void CSquareGrid::SetModelMatrix(const glm::mat4& mat)
	{
		m_modelMat = mat;
		m_shader->setMat4("u_model", m_modelMat);
	}

	void CSquareGrid::Update(const glm::mat4& view)
	{
        m_shader->setMat4(glGetUniformLocation(*m_shader, "u_view"), view);

		if(m_hasWaves)
			m_shader->setFloat(glGetUniformLocation(m_shader->getProgramID(), "u_time"), (float)glfwGetTime());
		else
			m_shader->setFloat(glGetUniformLocation(m_shader->getProgramID(), "u_time"), 1.0f); //this freezes time and with it, the waves

        m_shader->setVec3(glGetUniformLocation(m_shader->getProgramID(), "u_cameraPos"), MovableCamera::getInstance().Position);

		m_transformGizmo.Update(view, glm::mat4{ 1.0f });
	}

    void CSquareGrid::Draw(bool transparent /*= false*/)
	{
		if (!m_drawable)
			DrawInit();

        if (transparent)
            m_shader->setFloat(
                glGetUniformLocation(m_shader->getProgramID(), "u_transparency"),
                glm::sin((float)glfwGetTime())
			);
		else if (m_drawType == GL_LINES)
		{
			float alpha = 0.2f;
            m_shader->setFloat(
				glGetUniformLocation(m_shader->getProgramID(), "u_transparency"),
				alpha
			);
		}
		else
		{
            m_shader->setFloat(
				glGetUniformLocation(m_shader->getProgramID(), "u_transparency"),
				1.0f
			);
		}

		m_transformGizmo.Draw();

        m_shader->use();
		glBindVertexArray(m_vao);
		glDrawElements(m_drawType, m_indicesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
        m_shader->disable();
	}

	void CSquareGrid::CreateWavesOnGrid()
	{
		m_hasWaves = true;
		// UniformBlock test
        m_shader->use();

		//float lambda = 0.5f;
		//float K = 2 * glm::pi<float>() / lambda;
		float K = 15.0f; // number of waves
		float velocity = 0.05f;
		float amplitude = 5.0f;

		const auto& programAlias = m_shader->getProgramID();
		unsigned int blockId = glGetUniformBlockIndex(programAlias, "SineWaveSettings");

		int blockSize;
		glGetActiveUniformBlockiv(programAlias, blockId, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		GLbyte* blockBuffer = (GLbyte*)malloc(blockSize);

		const char* const ubNames[3]{
			"K",
			"velocity",
			"amplitude"
		};
		GLuint handles[3];
		glGetUniformIndices(programAlias, 3, ubNames, handles);
		GLint offsets[3];
		glGetActiveUniformsiv(programAlias, 3, handles, GL_UNIFORM_OFFSET, &offsets[0]);

		memcpy(blockBuffer + offsets[0], &K, sizeof(float));
		memcpy(blockBuffer + offsets[1], &velocity, sizeof(float));
		memcpy(blockBuffer + offsets[2], &amplitude, sizeof(float));

		unsigned int ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

		glBindBufferBase(GL_UNIFORM_BUFFER, blockId, ubo);

        m_shader->disable();

		free(blockBuffer);
	}

	void CSquareGrid::EnableWaves()
	{
		m_hasWaves = true;
		m_shader->setFloat(glGetUniformLocation(m_shader->getProgramID(), "u_time"), 1.0f);
	}

	void CSquareGrid::DisableWaves()
	{
		m_hasWaves = false;
	}

	void CSquareGrid::GenerateVertices()
	{
		const auto& A = defaultGrid[0];

		for (unsigned int j = 0; j < m_resolution + 1; j++)
		{
			for (unsigned int i = 0; i < m_resolution + 1; i++)
			{
				const auto translation = glm::vec3{
					((float)i / m_resolution)*2.0f,     // multiplied by 2.0f because that's the arbitrary size of default grid
					(-((float)j) / m_resolution)*2.0f,  // multiplied by 2.0f because that's the arbitrary size of default grid
					(float)0.0f
				};
				m_vertices.push_back(A + translation);
			}
		}
	}

	void CSquareGrid::GenerateIndices()
	{
		const auto& A = defaultGrid[0];

		for (unsigned int j = 0; j < m_resolution; j++)
		{
			int startIndex = j * (m_resolution + 1);
			for (unsigned int i = startIndex, loopCounter = 0; loopCounter < m_resolution; i++, loopCounter++)
			{
				m_indices.push_back(i);
				m_indices.push_back(i + 1);
				m_indices.push_back(i + 1 + m_resolution);

				m_indices.push_back(i + 1 + m_resolution);
				m_indices.push_back(i + 1);
				m_indices.push_back(i + 2 + m_resolution);
			}
		}
	}

	void CSquareGrid::DrawInit()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(2, m_buffers);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);

		m_indicesCount = static_cast<unsigned int>(m_indices.size());

		m_drawable = true;

		if (m_eraseFromRAM)
		{
			m_vertices.clear();
			m_indices.clear();

			m_indices.shrink_to_fit();
			m_vertices.shrink_to_fit();
		}

        m_shader->setVec3("u_lightPos", g_sunPosition);
	}

}
