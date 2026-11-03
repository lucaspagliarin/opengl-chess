
#include "Prisma.h"

// Prisma triangular: duas bases triangulares e tres faces retangulares.
// Eixo de comprimento: Z.
// Altura: eixo Y.
// Largura: eixo X.

float verticesPrisma[] = {
    // Face frontal (Z positivo)
    // pos X, Y, Z       // UV
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 0 - base esquerda
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 1 - base direita
     0.0f,  0.5f,  0.5f,  0.5f, 0.0f, // 2 - topo

    // Face traseira (Z negativo)
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 3
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 4
     0.0f,  0.5f, -0.5f,  0.5f, 0.0f  // 5
};

unsigned int indicesPrisma[] = {
    // Triangulo frontal
    0, 1, 2,

    // Triangulo traseiro (orientacao invertida)
    5, 4, 3,

    // Face inferior
    3, 4, 1,
    1, 0, 3,

    // Face lateral esquerda
    3, 0, 2,
    2, 5, 3,

    // Face lateral direita
    1, 4, 5,
    5, 2, 1
};

Prisma::Prisma(
    glm::vec3 pos,
    glm::vec3 rot,
    glm::vec3 scl,
    float ang
)
    : Object(pos, rot, scl, ang),
      VAO(0),
      VBO(0),
      EBO(0)
{
    init();
}

void Prisma::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesPrisma),
        verticesPrisma,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indicesPrisma),
        indicesPrisma,
        GL_STATIC_DRAW
    );

    // Posicao
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Coordenadas de textura
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Prisma::draw(Shader& shader, glm::mat4 parentTransform)
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

    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        sizeof(indicesPrisma) / sizeof(indicesPrisma[0]),
        GL_UNSIGNED_INT,
        0
    );
    glBindVertexArray(0);
}
