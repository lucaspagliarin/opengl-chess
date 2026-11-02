
#include "Group.h"

Group::Group(
    glm::vec3 pos,
    glm::vec3 rot,
    glm::vec3 scl,
    float ang
) : Object(pos, rot, scl, ang) {
}

void Group::addChild(Object* child) {
    if (child != nullptr) {
        children.push_back(child);
    }
}

void Group::draw(
    Shader& shader,
    glm::mat4 parentTransform
) {
    glm::mat4 model = parentTransform;

    model = glm::translate(model, position);

    if (angle != 0.0f && glm::length(rotation) > 0.0f) {
        model = glm::rotate(
            model,
            glm::radians(angle),
            glm::normalize(rotation)
        );
    }

    model = glm::scale(model, scale);

    for (Object* child : children) {
        child->draw(shader, model);
    }
}

