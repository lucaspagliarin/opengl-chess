
#pragma once

#include "Group.h"
#include "Cylinder.h"
#include "Sphere.h"

class Bishop : public Group {
public:
    Bishop(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

private:
    Cylinder base;
    Cylinder body;
    Cylinder collar;
    Cylinder head_bottom;
    Cylinder head_top;
    Sphere tip;
};

