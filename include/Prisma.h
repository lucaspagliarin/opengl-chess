
#pragma once

#include "Object.h"

class Prisma : public Object {
public:
    Prisma(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 rot = glm::vec3(0.0f),
        glm::vec3 scl = glm::vec3(1.0f),
        float ang = 0.0f
    );

    void draw(Shader& shader, glm::mat4 parentTransform) override;

private:
    unsigned int VAO, VBO, EBO;

    void init();
};
