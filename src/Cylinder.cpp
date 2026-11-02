
#include "Cylinder.h"

#include <GL/glew.h>
#include <cmath>
#include <vector>
#include <algorithm>

Cylinder::Cylinder(
    glm::vec3 pos,
    float bottomRadius,
    float topRadius,
    float height,
    int segments,
    glm::vec4 color
)
    : Object(pos, glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec3(1.0f), 0.0f),
      VAO(0),
      VBO(0),
      EBO(0),
      indexCount(0),
      bottomRadius(std::max(0.0f, bottomRadius)),
      topRadius(std::max(0.0f, topRadius)),
      height(std::max(0.001f, height)),
      segments(std::max(3, segments)),
      color(color)
{
    init();
}

Cylinder::~Cylinder()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cylinder::init()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;
    const float halfHeight = height / 2.0f;

    // Cada vértice possui:
    // posição X, Y, Z e coordenadas de textura U, V.
    auto addVertex = [&](float x, float y, float z,
                         float u, float v)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        vertices.push_back(u);
        vertices.push_back(v);
    };

    // 1. Superfície lateral.
    // Criamos um anel inferior e outro superior.
    for (int i = 0; i <= segments; i++)
    {
        float t = static_cast<float>(i) / segments;
        float theta = 2.0f * PI * t;

        float c = std::cos(theta);
        float s = std::sin(theta);

        addVertex(
            bottomRadius * c, -halfHeight,
            bottomRadius * s, t, 0.0f
        );

        addVertex(
            topRadius * c, halfHeight,
            topRadius * s, t, 1.0f
        );
    }

    // Liga os anéis formando os triângulos laterais.
    for (int i = 0; i < segments; i++)
    {
        unsigned int b0 = i * 2;
        unsigned int t0 = b0 + 1;
        unsigned int b1 = b0 + 2;
        unsigned int t1 = b0 + 3;

        indices.push_back(b0);
        indices.push_back(t0);
        indices.push_back(b1);

        indices.push_back(b1);
        indices.push_back(t0);
        indices.push_back(t1);
    }

    // 2. Tampa inferior.
    unsigned int bottomCenter =
        static_cast<unsigned int>(vertices.size() / 5);

    addVertex(0.0f, -halfHeight, 0.0f, 0.5f, 0.5f);

    unsigned int bottomRing =
        static_cast<unsigned int>(vertices.size() / 5);

    for (int i = 0; i <= segments; i++)
    {
        float theta = 2.0f * PI * i / segments;
        float c = std::cos(theta);
        float s = std::sin(theta);

        addVertex(
            bottomRadius * c,
            -halfHeight,
            bottomRadius * s,
            0.5f + 0.5f * c,
            0.5f + 0.5f * s
        );
    }

    for (int i = 0; i < segments; i++)
    {
        indices.push_back(bottomCenter);
        indices.push_back(bottomRing + i + 1);
        indices.push_back(bottomRing + i);
    }

    // 3. Tampa superior.
    unsigned int topCenter =
        static_cast<unsigned int>(vertices.size() / 5);

    addVertex(0.0f, halfHeight, 0.0f, 0.5f, 0.5f);

    unsigned int topRing =
        static_cast<unsigned int>(vertices.size() / 5);

    for (int i = 0; i <= segments; i++)
    {
        float theta = 2.0f * PI * i / segments;
        float c = std::cos(theta);
        float s = std::sin(theta);

        addVertex(
            topRadius * c,
            halfHeight,
            topRadius * s,
            0.5f + 0.5f * c,
            0.5f + 0.5f * s
        );
    }

    for (int i = 0; i < segments; i++)
    {
        indices.push_back(topCenter);
        indices.push_back(topRing + i);
        indices.push_back(topRing + i + 1);
    }

    indexCount = static_cast<unsigned int>(indices.size());

    // Envia os vértices e índices para a GPU.
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

    // Atributo 0: posição XYZ.
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Atributo 1: coordenadas UV.
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Cylinder::draw(Shader& shader, glm::mat4 model)
{
    // Aplica as transformações do objeto.
    model = glm::translate(model, position);

    if (angle != 0.0f)
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

    // Ativa o modo de cor sólida somente para este objeto.
    shader.setInt("useSolidColor", 1);
    shader.setVec4("solidColor", color);

    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );
    glBindVertexArray(0);

    // Restaura o modo de textura para os próximos objetos.
    shader.setInt("useSolidColor", 0);
}

