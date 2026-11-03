#pragma once

#include "Group.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "Prisma.h"
#include "Cube.h"

class Knight : public Group {
public:
    Knight(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

private:
    Cylinder base;

    Cylinder body;

    Cube head;

    Pyramid earLeft;
    Pyramid earRight;
};
