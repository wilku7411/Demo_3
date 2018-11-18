#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

namespace nshapes
{

    // vertex array used to create some simple shapes
    class vao
    {
    public:
        unsigned int buffers[3];
        unsigned int vertexArray;
        unsigned int indicesCount;
        unsigned int instancesCount;

        vao();
        ~vao();

        void clearVertexArray();
        void createLine(const glm::vec3& A, const glm::vec3& B, const glm::vec3& color);
        void createAsteroidGamePlaceholder();
        void createAsteroidGameShip();
		void createAsteroidGameProjectiles(const unsigned int count);
        void updateAsteroidGameProjectiles(const std::vector<glm::vec3>& translations);
        void createQuad();
        void createSimpleOctagon();

    private:
        void generateOctagon(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);
    };
}


