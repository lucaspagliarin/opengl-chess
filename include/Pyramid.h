
#pragma once

#include "Shader.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Pyramid : public Object {
public:
    Pyramid(
        glm::vec3 pos = glm::vec3(0.0f),
        float baseSize = 2.0f,
        float height = 2.0f,
        glm::vec4 color = glm::vec4(0.2f, 0.7f, 0.3f, 1.0f)
    );

    ~Pyramid() override;

    void draw(Shader& shader, glm::mat4 parentTransform) override;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    float baseSize;
    float height;
    glm::vec4 color;

    void init();
};

