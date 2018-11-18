#pragma once

#include "../graphics/Shader.h"
#include "../graphics/OriginGizmo.h"
#include "../IO/Window.h"

#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <vector>

namespace NGrid
{
	const glm::vec3 defaultGrid[2] = {
		{ -1.0f,  1.0f,  0.0f },
		{ 1.0f, -1.0f,  0.0f }
	};

	class CSquareGrid
	{
	public:
		CSquareGrid(const unsigned int res, const unsigned int scaleMultiplier = 1, bool eraseFromRAM = true);

		void GenerateGrid();
		void Update(const glm::mat4& view);
		void Draw(bool transparent = false);

		void SetDrawType(unsigned int type);
        void SetProjectionMatrix(const glm::mat4& projection);

		void GetModelMatrix(glm::mat4& out_mat);
		void SetModelMatrix(const glm::mat4& mat);

		// Simulates simple waves on grind, usable anytime
		void CreateWavesOnGrid();
		void EnableWaves();
		void DisableWaves();

	private:

		// size is W = 2.0f, H = 2.0f
		// resolution = 3 example
		//            ^
		//            |Yaxis
		//    A----------------
		//    |    |  |  |    |
		//    |    |  |  |    |
		//    |    |  |  |    |
		//    -----------------
		//    |    |  |  |    |
		//  --|----|-----|----|----> Xaxis
		//    |    |  |  |    |    
		//    ------------------
		//    |    |  |  |    |
		//    |    |  |  |    |
		//    |    |  |  |    |
		//    ----------------B
		void GenerateVertices();

		// abc are indices for triangle face
		//      3x3 example 
		//    a----b----0----0
		//    |   /|   /|   /|
		//    |  / |  / |  / |
		//    | /  | /  | /  |
		//    c----0----0----0
		//    |   /|   /|   /|
		//    |  / |  / |  / |
		//    | /  | /  | /  |    
		//    0----0----0----0
		//    |   /|   /|   /|
		//    |  / |  / |  / |
		//    | /  | /  | /  |
		//    0----0----0----0
		void GenerateIndices();

		void DrawInit();

		std::unique_ptr<CShader>    m_shader;
		COriginGizmo				m_transformGizmo;
		glm::mat4					m_modelMat;
		std::vector<glm::vec3>		m_vertices;
		std::vector<unsigned int>	m_indices;
		unsigned int				m_indicesCount;
		unsigned int				m_resolution;
		unsigned int				m_vao;
		unsigned int				m_buffers[2];
		unsigned int				m_drawType;
		bool						m_drawable;
		bool						m_eraseFromRAM;
		bool						m_hasWaves;
	};
};
