
#include "Queen.h"

Queen::Queen(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.34f, 0.34f, 0.20f, 32, color
      ),
      body(
          glm::vec3(0.0f, 0.55f, 0.0f),
          0.24f, 0.14f, 0.70f, 32, color
      ),
      head(
          glm::vec3(0.0f, 1.02f, 0.0f),
          0.17f, 32, 16, color
      )
{
    addChild(&base);
    addChild(&body);
    addChild(&head);
}

