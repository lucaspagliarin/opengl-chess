
#pragma once

#include "Shader.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Sphere : public Object {
public:
    Sphere(
        glm::vec3 pos = glm::vec3(0.0f),
        float radius = 1.0f,
        int sectors = 32,
        int stacks = 16,
        glm::vec4 color = glm::vec4(0.2f, 0.7f, 0.3f, 1.0f)
    );

    ~Sphere() override;

    void draw(Shader& shader, glm::mat4 model) override;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount;

    float radius;
    int sectors;
    int stacks;
    glm::vec4 color;

    void init();
};

