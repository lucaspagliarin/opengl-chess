
#include "Rook.h"
#include <cmath>

Rook::Rook(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.35f, 0.35f, 0.20f, 32, color
      ),
      body(
          glm::vec3(0.0f, 0.45f, 0.0f),
          0.22f, 0.22f, 0.50f, 32, color
      ),
      head(glm::vec3(0.0f, 0.76f, 0.0f)),
      crown(
          glm::vec3(0.0f, 0.0f, 0.0f),
          0.30f, 0.30f, 0.12f, 32, color
      )
{
    addChild(&base);
    addChild(&body);

    head.addChild(&crown);


    const int count = 8;
    const float radius = 0.24f;
    const float blockSize = 0.09f;
    const float blockHeight = 0.14f;

    battlements.reserve(count);

    for (int i = 0; i < count; i++)
    {
        float angle = 2.0f * 3.14159265f * i / count;

        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);

        battlements.emplace_back(
            glm::vec3(x, 0.12f, z),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(blockSize, blockHeight, blockSize),
            glm::degrees(angle),
            color,
            true
        );
    }


    for (Cube& block : battlements)
    {
        head.addChild(&block);
    }

    addChild(&head);
}
