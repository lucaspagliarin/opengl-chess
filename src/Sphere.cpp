
#include "Sphere.h"

#include <GL/glew.h>
#include <vector>
#include <cmath>
#include <algorithm>

Sphere::Sphere(
    glm::vec3 pos,
    float radius,
    int sectors,
    int stacks,
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
    radius(std::max(0.001f, radius)),
    sectors(std::max(3, sectors)),
    stacks(std::max(2, stacks)),
    color(color)
{
    init();
}

Sphere::~Sphere()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sphere::init()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;

    // Gera os vertices da esfera
    for (int i = 0; i <= stacks; i++)
    {
        float stackAngle = PI / 2.0f
                         - i * PI / stacks;

        float xy = radius * cos(stackAngle);
        float z = radius * sin(stackAngle);

        for (int j = 0; j <= sectors; j++)
        {
            float sectorAngle =
                j * 2.0f * PI / sectors;

            float x = xy * cos(sectorAngle);
            float y = xy * sin(sectorAngle);

            // Posicao XYZ
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Coordenadas UV
            vertices.push_back(
                (float)j / sectors
            );
            vertices.push_back(
                (float)i / stacks
            );
        }
    }

    // Gera os indices dos triangulos
    for (int i = 0; i < stacks; i++)
    {
        unsigned int k1 =
            i * (sectors + 1);

        unsigned int k2 =
            k1 + sectors + 1;

        for (int j = 0; j < sectors; j++, k1++, k2++)
        {
            // Triangulos da parte superior
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // Triangulos da parte inferior
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    indexCount = (unsigned int)indices.size();

    // Cria VAO, VBO e EBO
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

    // Atributo 0: posicao XYZ
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Atributo 1: coordenadas UV
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Sphere::draw(
    Shader& shader,
    glm::mat4 model
)
{
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

    // Ativa a cor solida
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

    // Restaura o modo de textura
    shader.setInt("useSolidColor", 0);
}

