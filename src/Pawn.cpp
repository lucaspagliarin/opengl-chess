#include "Pawn.h"

Pawn::Pawn(glm::vec3 pos, glm::vec4 color)
    : Group(pos),

      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.32f,
          0.32f,
          0.20f,
          32,
          color
      ),

      body(
          glm::vec3(0.0f, 0.38f, 0.0f),
          0.18f,
          0.13f,
          0.40f,
          32,
          color
      ),

      collar(
        glm::vec3(0.0f, 0.60f, 0.0f),
        0.19f, 0.19f,
        0.08f, 32, color
      ),

      head(
          glm::vec3(0.0f, 0.75f, 0.0f),
          0.17f,
          32,
          16,
          color
      )
{
    addChild(&base);
    addChild(&body);
    addChild(&collar);
    addChild(&head);
}
