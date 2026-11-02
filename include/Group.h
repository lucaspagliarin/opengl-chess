#ifndef GROUP_H
#define GROUP_H

#include "Object.h"
#include <vector>

class Group : public Object {
public:
    Group(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec3 rot = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3 scl = glm::vec3(1.0f),
        float ang = 0.0f
    );

    void addChild(Object* child);

    void draw(
        Shader& shader,
        glm::mat4 parentTransform
    ) override;

private:
    std::vector<Object*> children;
};

#endif
