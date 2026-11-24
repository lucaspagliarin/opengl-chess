
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

    // Normais das faces laterais. Cada face � inclinada, ent�o a normal
    // tem uma parte horizontal (para fora) e uma parte vertical (para cima).
    glm::vec3 nFront = glm::normalize(glm::vec3(0.0f,       halfBase, -height));
    glm::vec3 nRight = glm::normalize(glm::vec3(height,     halfBase,  0.0f));
    glm::vec3 nBack  = glm::normalize(glm::vec3(0.0f,       halfBase,  height));
    glm::vec3 nLeft  = glm::normalize(glm::vec3(-height,    halfBase,  0.0f));

    // Cada v�rtice cont�m XYZ + UV + normal (8 floats, igual ao Cube).
    // Como cada face tem sua pr�pria normal, os v�rtices dos cantos s�o
    // duplicados: um conjunto para a base e um para cada face lateral.
    float vertexData[] = {
        // Posi��o X, Y, Z                U, V         Normal

        // Base (normal para baixo)
        -halfBase, -halfHeight, -halfBase, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,  // 0
         halfBase, -halfHeight, -halfBase, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  // 1
         halfBase, -halfHeight,  halfBase, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // 2
        -halfBase, -halfHeight,  halfBase, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // 3

        // Face frontal (Z negativo)
        -halfBase, -halfHeight, -halfBase, 0.0f, 0.0f,  nFront.x, nFront.y, nFront.z,  // 4
         halfBase, -halfHeight, -halfBase, 1.0f, 0.0f,  nFront.x, nFront.y, nFront.z,  // 5
         0.0f,      halfHeight,  0.0f,     0.5f, 0.5f,  nFront.x, nFront.y, nFront.z,  // 6

        // Face lateral direita (X positivo)
         halfBase, -halfHeight, -halfBase, 1.0f, 0.0f,  nRight.x, nRight.y, nRight.z,  // 7
         halfBase, -halfHeight,  halfBase, 1.0f, 1.0f,  nRight.x, nRight.y, nRight.z,  // 8
         0.0f,      halfHeight,  0.0f,     0.5f, 0.5f,  nRight.x, nRight.y, nRight.z,  // 9

        // Face traseira (Z positivo)
         halfBase, -halfHeight,  halfBase, 1.0f, 1.0f,  nBack.x, nBack.y, nBack.z,  // 10
        -halfBase, -halfHeight,  halfBase, 0.0f, 1.0f,  nBack.x, nBack.y, nBack.z,  // 11
         0.0f,      halfHeight,  0.0f,     0.5f, 0.5f,  nBack.x, nBack.y, nBack.z,  // 12

        // Face lateral esquerda (X negativo)
        -halfBase, -halfHeight,  halfBase, 0.0f, 1.0f,  nLeft.x, nLeft.y, nLeft.z,  // 13
        -halfBase, -halfHeight, -halfBase, 0.0f, 0.0f,  nLeft.x, nLeft.y, nLeft.z,  // 14
         0.0f,      halfHeight,  0.0f,     0.5f, 0.5f,  nLeft.x, nLeft.y, nLeft.z   // 15
    };

    vertices.assign(
        vertexData,
        vertexData + sizeof(vertexData) / sizeof(float)
    );

    indices = {
        // Base quadrada: 2 tri�ngulos.
        0, 2, 1,
        0, 3, 2,

        // Face frontal
        4, 6, 5,

        // Face lateral direita
        7, 9, 8,

        // Face traseira
        10, 12, 11,

        // Face lateral esquerda
        13, 15, 14
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

    // Posi��o XYZ.
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Coordenadas UV.
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Normais.
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(5 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

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

