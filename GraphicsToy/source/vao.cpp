#include "vao.h"



nshapes::vao::vao()
    : buffers{ 0,0,0 }
    , vertexArray(0)
    , indicesCount(0)
    , instancesCount(0)
{}

nshapes::vao::~vao()
{
    clearVertexArray();
}

void nshapes::vao::clearVertexArray()
{
    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &vertexArray);
}

void nshapes::vao::createLine(const glm::vec3& A, const glm::vec3& B, const glm::vec3& color)
{
    std::vector<float> vertices{
        A.x, A.y, A.z, color.r, color.g, color.b,
        B.x, B.y, B.z, color.r, color.g, color.b
    };

    std::vector<unsigned int> indices{
        0,1
    };

    indicesCount = indices.size();

    if (vertexArray != 0)
        clearVertexArray();

    glGenVertexArrays(1, &vertexArray);

    glBindVertexArray(vertexArray);
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void nshapes::vao::createAsteroidGamePlaceholder()
{
    std::vector<float> vertices{
        // xyz					//rgb
       -0.5f, -0.5f, 0.0f,	    1.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, 0.0f,	    1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, 0.0f,	    1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,	    1.0f, 0.0f, 0.0f
    };

    std::vector<unsigned int> indices{
        0,1,2,
        0,2,3
    };

    indicesCount = indices.size();

    if (vertexArray != 0)
        clearVertexArray();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void nshapes::vao::createAsteroidGameShip()
{
    std::vector<float> vertices{
        // xyz					//rgb
        -0.6f, -0.6f,  0.0f,	1.0f, 1.0f, 1.0f,
         0.0f,  1.0f,  0.0f,	1.0f, 1.0f, 1.0f,
         0.6f, -0.6f,  0.0f,	1.0f, 1.0f, 1.0f,
         0.0f, -0.2f,  0.0f,	1.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices{
        0,1,
1,2,
2,3,
3,0
    };

    indicesCount = indices.size();

    if (vertexArray != 0)
        clearVertexArray();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBindVertexArray(0);
}

void nshapes::vao::createAsteroidGameProjectiles(const unsigned int count)
{
    instancesCount = count;

    std::vector<float> vertices{
        // xyz				//rgb
        0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices{
        0,1,2,
    };
    indicesCount = indices.size();

    if (vertexArray != 0)
        clearVertexArray();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // translation for each projectile will be updated each loop
    // so the size I need to reserve for incoming data will be:
    // (translations_count * translation_dimention * sizeof_element)
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, instancesCount * 3 * sizeof(float), NULL, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBindVertexArray(0);
}

void nshapes::vao::updateAsteroidGameProjectiles(const std::vector<glm::vec3>& translations)
{
    if (buffers[1] == 0)
    {
        printf("Error, make sure the buffer for projectiles has been created!");
        return;
    }

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, instancesCount * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instancesCount * 3 * sizeof(float), translations.data());
}

void nshapes::vao::createQuad()
{
    std::vector<float> vertices{
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices{
        0,1,2,
        0,2,3
    };

    indicesCount = indices.size();

    if (vertexArray != 0)
        clearVertexArray();

    glGenVertexArrays(1, &vertexArray);

    glBindVertexArray(vertexArray);
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void nshapes::vao::createSimpleOctagon()
{
    std::vector<glm::vec3> vertices{ 8 + 1 };
    std::vector<unsigned int> indices{ 8 * 3 };

    indicesCount = indices.size();

    generateOctagon(vertices, indices);

    if (vertexArray != 0)
        clearVertexArray();

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(2, buffers);

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void nshapes::vao::generateOctagon(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
{
    glm::vec3 origin{ 0.0f, 0.0f, 0.0f };
    float radius = 1.0f;

    for (int i = 0; i < vertices.capacity(); i++)
    {
        if (i == 0)
        {
            vertices.at(0) = glm::vec3{ 0.0f, 0.0f, 0.0f };
            continue;
        }

        glm::vec3 v;
        v.x = glm::cos(glm::radians(45.0f * (i - 1)) * radius);
        v.y = 0.0f;
        v.z = glm::sin(glm::radians(45.0f * (i - 1)) * radius);

        vertices.at(i) = v;
    }

    indices = std::vector<unsigned int>{
        0,1,2,
        0,2,3,
        0,3,4,
        0,4,5,
        0,5,6,
        0,6,7,
        0,7,8,
        0,8,1
    };
}
