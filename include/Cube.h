
#pragma once

#include "Object.h"

class Cube : public Object {
public:
    Cube(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 rot = glm::vec3(0.0f),
        glm::vec3 scl = glm::vec3(1.0f),
        float ang = 0.0f,
        glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f),
        bool useColor = false
    );

    void setColor(glm::vec4 newColor);

    void draw(Shader& shader, glm::mat4 parentTransform) override;

private:
    unsigned int VAO, VBO, EBO;

    glm::vec4 color;
    bool useColor;

    void init();
};
