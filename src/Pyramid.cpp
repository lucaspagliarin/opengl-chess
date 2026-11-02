
#include "Pyramid.h"

#include <GL/glew.h>
#include <vector>
#include <algorithm>

Pyramid::Pyramid(
    glm::vec3 pos,
    float baseSize,
    float height,
    glm::vec4 color
)
    : Object(
        pos,
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f),
        0.0f
    ),
      VAO(0),
      VBO(0),
      EBO(0),
      indexCount(0),
      baseSize(std::max(0.001f, baseSize)),
      height(std::max(0.001f, height)),
      color(color)
{
    init();
}

Pyramid::~Pyramid()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Pyramid::init()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float halfBase = baseSize / 2.0f;
    float halfHeight = height / 2.0f;

    // Cada vértice contém XYZ + UV.
    // 4 cantos da base e 1 ponta superior.
    float vertexData[] = {
        // Posição X, Y, Z     U, V

        -halfBase, -halfHeight, -halfBase, 0.0f, 0.0f,
         halfBase, -halfHeight, -halfBase, 1.0f, 0.0f,
         halfBase, -halfHeight,  halfBase, 1.0f, 1.0f,
        -halfBase, -halfHeight,  halfBase, 0.0f, 1.0f,

         0.0f,      halfHeight, 0.0f,      0.5f, 0.5f
    };

    vertices.assign(
        vertexData,
        vertexData + sizeof(vertexData) / sizeof(float)
    );

    // Base quadrada: 2 triângulos.
    indices = {
        0, 2, 1,
        0, 3, 2,

        // Face frontal
        0, 4, 1,

        // Face lateral direita
        1, 4, 2,

        // Face traseira
        2, 4, 3,

        // Face lateral esquerda
        3, 4, 0
    };

    indexCount = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );

    // Posição XYZ.
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Coordenadas UV.
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Pyramid::draw(Shader& shader, glm::mat4 parentTransform)
{
    glm::mat4 model = parentTransform;

    model = glm::translate(model, position);

    if (angle != 0.0f && glm::length(rotation) > 0.0f)
    {
        model = glm::rotate(
            model,
            glm::radians(angle),
            glm::normalize(rotation)
        );
    }

    model = glm::scale(model, scale);

    shader.use();
    shader.setMat4("model", model);

    shader.setInt("useSolidColor", 1);
    shader.setVec4("solidColor", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader.setInt("useSolidColor", 0);
}

