#pragma once

#include "Shader.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cylinder : public Object {
public:
    Cylinder(
        glm::vec3 pos = glm::vec3(0.0f),
        float bottomRadius = 1.0f,
        float topRadius = 1.0f,
        float height = 2.0f,
        int segments = 32,
        glm::vec4 color = glm::vec4(0.2f, 0.7f, 0.3f, 1.0f)
    );

    ~Cylinder() override;

    void draw(Shader& shader, glm::mat4 model) override;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    float bottomRadius;
    float topRadius;
    float height;
    int segments;
    glm::vec4 color;

    void init();
};
