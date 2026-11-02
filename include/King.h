

#pragma once

#include "Group.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Cube.h"

class King : public Group {
public:
    King(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

private:
    Cylinder base;
    Cylinder body;
    Sphere head;

    Cube crossVertical;
    Cube crossHorizontal;
};
