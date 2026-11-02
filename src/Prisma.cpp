// Prisma.cpp
#include "Prisma.h"

float verticesPrisma[] = {
    // positions          // tex coords
    // back face
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    // front face
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    // left face
   -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    // right face
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    // top face
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    // bottom face
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
};

unsigned int indicesPrisma[] = {
    0,1,2, 2,3,0,        // back
    4,5,6, 6,7,4,        // front
    8,9,10, 10,11,8,     // left
    12,13,14, 14,15,12,  // right
    16,17,18, 18,19,16,  // top
    20,21,22, 22,23,20   // bottom
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPrisma), verticesPrisma, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPrisma), indicesPrisma, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
