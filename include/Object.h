#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"

class Object {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float angle;

    Object();
    Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float ang);

    virtual void draw(Shader& shader, glm::mat4 parentTransform);
    virtual ~Object() {}
};

#endif
