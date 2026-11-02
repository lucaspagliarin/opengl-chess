

#include "King.h"

King::King(glm::vec3 pos, glm::vec4 color)
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
          0.14f, 32, 16, color
      ),
      crossVertical(
          glm::vec3(0.0f, 1.28f, 0.0f),
          glm::vec3(0.0f),
          glm::vec3(0.08f, 0.30f, 0.08f)
      ),
      crossHorizontal(
          glm::vec3(0.0f, 1.31f, 0.0f),
          glm::vec3(0.0f),
          glm::vec3(0.24f, 0.08f, 0.08f)
      )
{
    addChild(&base);
    addChild(&body);
    addChild(&head);

    addChild(&crossVertical);
    addChild(&crossHorizontal);
}
