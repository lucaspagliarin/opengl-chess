
#pragma once

#include "Group.h"
#include "Cylinder.h"
#include "Cube.h"
#include <vector>

class Rook : public Group {
public:
    Rook(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

private:
    Cylinder base;
    Cylinder body;

    Group head;
    Cylinder crown;

    std::vector<Cube> battlements;
};
